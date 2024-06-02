#include <windows.h>
#include <winuser.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


char gameWindowTitle[] = "Fuse";
const string ballFileName = "./ball.png";
const string paddleFileName = "./paddle.png";
WORD LEFT_KEY = 'C';
WORD RIGHT_KEY = 'M';
WORD lastKeyPress = '0';
const Scalar CWHITE(255, 255, 255);
const Scalar CGREEN(0, 255, 0);

void pressKey(WORD &key){
    /*
    Send a key press via SendInput. Uses scan code
    */
    INPUT iKey;
    iKey.type = INPUT_KEYBOARD;
    iKey.ki.wVk = key;
    UINT uSent = SendInput(1, &iKey, sizeof(iKey));
    if (uSent != 1){
         cout << "Failed to send input" << endl;
    }
};
void releaseKey(WORD &key){
    /*
    Release a key press via SendInput. Uses scan code
    */
    INPUT iKey;
    iKey.type = INPUT_KEYBOARD;
    iKey.ki.wVk = key;
    iKey.ki.dwFlags = KEYEVENTF_KEYUP;
    UINT uSent = SendInput(1, &iKey, sizeof(iKey));
    if (uSent != 1){
         cout << "Failed to send input" << endl;
    }
};

Mat hwnd2mat(HWND hwnd){
    /*
    Creates a BMP in memory of the window handle provided
    Copied from https://stackoverflow.com/questions/50395294/c-window-capture-output-isnt-the-same-size-as-said-window
    */
    HDC hwindowDC, hwindowCompatibleDC;

    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    Mat src;
    BITMAPINFOHEADER  bi;

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize;    // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    srcheight = windowsize.bottom;
    srcwidth = windowsize.right;
    height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
    width = windowsize.right / 1;

    src.create(height, width, CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

                                                                                                       // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
};
Point templateDetection(Mat &src, Mat &greyedSrc, Mat &templateMat, Scalar boxColor){
    /*
    Detects templace passed and draws a rectangle around it on source image
    We want to return the center of the passed template
    */
    Mat result;
    result.create(src.rows - templateMat.rows + 1, src.cols - templateMat.cols + 1, CV_32FC1);
    matchTemplate(greyedSrc, templateMat, result, TM_CCOEFF_NORMED);
    double maxVal;
    Point maxLoc;
    minMaxLoc(result, NULL, &maxVal, NULL, &maxLoc);
    rectangle(src, maxLoc, Point(maxLoc.x + templateMat.cols, maxLoc.y + templateMat.rows), boxColor, 2);
    maxLoc.x = maxLoc.x + (templateMat.cols / 2 );
    maxLoc.y = maxLoc.y + (templateMat.rows / 2 );
    return maxLoc;
};
int main(int argc, char* argv[]){

    //Load the templates and convert greyscale
    Mat ball = imread(ballFileName, IMREAD_GRAYSCALE);
    Mat paddle = imread(paddleFileName, IMREAD_GRAYSCALE);
    if (ball.empty() || paddle.empty()){
        cout << "Error reading template files" << endl;
        return 0;
    }
    //prepare the windows and check game window is open
    //switch to it if open
    string window_name = "OpenCV";
    namedWindow(window_name);
    HWND gameWindow = FindWindow(NULL, gameWindowTitle);
    if (!gameWindow){
        cout << "Game window not found" << endl;
        return 0;
    }
    else {
        cout << "Game window found : " << gameWindow << endl;
    }
    SetForegroundWindow(gameWindow);
    SetFocus(gameWindow );
    //setup for drawing text values on the screen
    int textFont = FONT_HERSHEY_PLAIN;
    double textScale = 1.0;
    Scalar textColor(CWHITE);
    Point text1Loc(10, 15);
    Point text2Loc(10, 30);
    Point text3Loc(10, 45);
    //setup for keeping track of previous ball locations
    Point oldBallLoc(0,0);
    Point predictedBallLoc(0, 0);
    //store last key press
    
    while (true) {
   
        Mat uncropped = hwnd2mat(gameWindow);
        Rect roi(80, 60, 370, 400);
            // Crop the frame
        Mat frame = uncropped(roi);
        Mat greyFrame;
        cvtColor(frame, greyFrame, COLOR_BGR2GRAY);

        Point ballLoc = templateDetection(frame, greyFrame, ball, CWHITE);
        Point paddleLoc = templateDetection(frame, greyFrame, paddle, CGREEN);
        Point diff = ballLoc - oldBallLoc;
        //if there's a massive difference then it's probably a false positive on the template match so do nothing
        if (abs(diff.x) < 60 || abs(diff.y) < 60){
             predictedBallLoc = ballLoc + (diff * 2);
             oldBallLoc = ballLoc;
        }
        else {
            predictedBallLoc = oldBallLoc;
        }
        //move predicted to paddle level if it's under the paddle
        if (predictedBallLoc.y > paddleLoc.y){
            predictedBallLoc.y = paddleLoc.y;
        }
        //if the ball is between the paddle center and one of it's sides then let go of the key so we don't overshoot too much

        if (predictedBallLoc.x < (paddleLoc.x - (paddle.rows / 3)) && predictedBallLoc.x > paddleLoc.x){
            releaseKey(lastKeyPress);
            lastKeyPress = LEFT_KEY;
        }
        else if (predictedBallLoc.x > paddleLoc.x && predictedBallLoc.x < paddleLoc.x + (paddle.rows / 3)){
            releaseKey(lastKeyPress);
            lastKeyPress = RIGHT_KEY;
        }
        if (predictedBallLoc.x < paddleLoc.x) {
            if (lastKeyPress != LEFT_KEY){
                releaseKey(lastKeyPress);
                lastKeyPress = LEFT_KEY;
            }
        }
        if (predictedBallLoc.x > paddleLoc.x) {
            if (lastKeyPress != RIGHT_KEY){
                releaseKey(lastKeyPress);
                lastKeyPress = RIGHT_KEY;
            }
        }
        
        pressKey(lastKeyPress);
        putText(frame, "Pressed key " + to_string(lastKeyPress), Point(10, 45), FONT_HERSHEY_PLAIN, 1.0, CWHITE);
        //add values to the frame for troubleshooting.
        putText(frame, "Paddle x : " + to_string(paddleLoc.x), text1Loc, textFont, textScale, textColor);
        putText(frame, "Ball x : " + to_string(ballLoc.x) + " Predicted ball x : " + to_string(predictedBallLoc.x), text2Loc, textFont, textScale, textColor);
        circle(frame, predictedBallLoc, 5, CWHITE, 2);

        //show the frame

        imshow(window_name, frame);
        //wait for for 10 ms until any key is pressed.  
        //If the 'Esc' key is pressed, break the while loop.
        //If the any other key is pressed, continue the loop 
        //If any key is not pressed withing 10 ms, continue the loop 
        if (waitKey(10) == 27){
            cout << "Esc key is pressed by user. Exiting program" << endl;
            break;
        }
        //If the game window exists and exit if not
        if (!IsWindow(gameWindow)){
            cout << "Game window was closed. Exiting program" << endl;
            break;
        };
    }
    destroyAllWindows();
    return 0;
}