#include <windows.h>
#include <winuser.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

WORD LEFT_KEY = 'C';
WORD RIGHT_KEY = 'M';
const char gameWindowTitle[] = "Fuse";
const string ballFileName = "./ball.png";
const string paddleFileName = "./paddle.png";


void pressKey(WORD &key){
    /*
    Send a key press via SendInput. Uses scan code
    */
    INPUT iKey;
    iKey.type = INPUT_KEYBOARD;
    iKey.ki.wVk = key;
    UINT uSent = SendInput(1, &iKey, sizeof(iKey));
    if (uSent != 1){
        cout << ("SendInput failed") << endl;
    }
}

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
}

int main(int argc, char* argv[]){

    //Load the templates and convert greyscale
    Mat ball = imread(ballFileName, IMREAD_GRAYSCALE);
    Mat paddle = imread(paddleFileName, IMREAD_GRAYSCALE);
    if (ball.empty() || paddle.empty()){
        cout << "Error reading template files" << endl;
        return 0;
    }
    //prepare the windows and check game window is open
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
    Scalar textColor(255, 255, 255);
    Point text1Loc(10, 10);
    Point text2Loc(10, 20);
    Point text3Loc(10, 30);
    //setup for keeping track of previous ball locations
    Point oldBallLoc(0,0);
   
    while (true) {
        
        Mat frame  = hwnd2mat(gameWindow);
        Mat greyFrame;
        cvtColor(frame, greyFrame, COLOR_BGR2GRAY);

        Mat ballResult;
        ballResult.create(frame.rows - ball.rows + 1, frame.cols - ball.rows + 1, CV_32FC1);
        matchTemplate(greyFrame, ball, ballResult, TM_CCOEFF_NORMED);
        // Find the maximum value and its location in the result image
        double max_val;
        Point max_loc;
        minMaxLoc(ballResult, NULL, &max_val, NULL, &max_loc);

        // Draw a rectangle around the best match location on the source image
        rectangle(frame, max_loc, Point(max_loc.x + ball.cols, max_loc.y + ball.rows), Scalar(255, 255, 255), 2);
        
        Mat paddleResult;
        paddleResult.create(frame.rows - paddle.rows + 1, frame.cols - paddle.rows + 1, CV_32FC1);
        matchTemplate(greyFrame, paddle, paddleResult, TM_CCOEFF_NORMED);
        minMaxLoc(paddleResult, NULL, &max_val, NULL, &max_loc);

        // Draw a rectangle around the best match location on the source image
        rectangle(frame, max_loc, Point(max_loc.x + paddle.cols, max_loc.y + paddle.rows), Scalar(0, 255, 0), 2);
        
        putText(frame, "Paddle x : " + to_string(max_loc.x), text1Loc, textFont, textScale, textColor);
        //show the frame in the created window
        imshow(window_name, frame);
        //pressKey(RIGHT_KEY);
        
        
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