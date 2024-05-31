#include <windows.h>
#include <winuser.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

WORD LEFT_KEY = 'C';
WORD RIGHT_KEY = 'M';
const char gameWindowTitle[] = "Fuse";

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


    string window_name = "OpenCV";
    namedWindow(window_name); //create a window called "My Camera Feed"
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

    for (int i = 0; i < 20; i++){
        pressKey(LEFT_KEY);
        pressKey(RIGHT_KEY);
    }
    while (true) {
        Mat frame  = hwnd2mat(gameWindow) ;

        
        //show the frame in the created window
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