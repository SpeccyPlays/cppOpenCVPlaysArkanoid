//Uncomment the following line if you are compiling this code in Visual Studio
//#include "stdafx.h"

#include <windows.h>
#include <winuser.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <tchar.h>

using namespace cv;
using namespace std;

char LEFT_KEY = 'C';
char RIGHT_KEY = 'M';
const char gameWindowTitle[] = "Untitled - Notepad";

int main(int argc, char* argv[]){
    //setup inputs
    //keep seperate for ease of keeping track
    
    //write a function that takes a char and presses down a key
    INPUT LEFT_KEY_DOWN;
    LEFT_KEY_DOWN.type = INPUT_KEYBOARD;
    LEFT_KEY_DOWN.ki.wVk = LEFT_KEY;
    INPUT RIGHT_KEY_DOWN;
    RIGHT_KEY_DOWN.type = INPUT_KEYBOARD;
    RIGHT_KEY_DOWN.ki.wVk = RIGHT_KEY;

    INPUT LEFT_KEY_UP;
    LEFT_KEY_DOWN.type = INPUT_KEYBOARD;
    LEFT_KEY_DOWN.ki.wVk = LEFT_KEY;
    LEFT_KEY_DOWN.ki.dwFlags = KEYEVENTF_KEYUP;

    INPUT RIGHT_KEY_UP;
    RIGHT_KEY_UP.type = INPUT_KEYBOARD;
    RIGHT_KEY_UP.ki.wVk = RIGHT_KEY;
    RIGHT_KEY_UP.ki.dwFlags = KEYEVENTF_KEYUP;

    //Open the default video camera
    /*VideoCapture cap(0);
    
    // if not success, exit program
    if (cap.isOpened() == false) {
        cout << "Cannot open the video camera" << endl;
        cin.get(); //wait for any key press
        return -1;
    } 

    double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Resolution of the video : " << dWidth << " x " << dHeight << endl;

    string window_name = "My Camera Feed";
    namedWindow(window_name); //create a window called "My Camera Feed"
    */HWND hActiveWindow = FindWindow(NULL, gameWindowTitle);
    cout << hActiveWindow << endl;
    SetForegroundWindow(hActiveWindow);
    SetFocus(hActiveWindow);

    //for (int i = 0; i < 20; i++){
        UINT uSent = SendInput(1, &LEFT_KEY_DOWN, sizeof(LEFT_KEY_DOWN));
        if (uSent != ARRAYSIZE(keyDown)){
            cout << ("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError())) << endl;
        }
    //}
    Sleep (5000);
    //UINT uSent1 = SendInput(ARRAYSIZE(keyUp), keyUp, sizeof(keyUp));
    /*while (true) {
        Mat frame;
        bool bSuccess = cap.read(frame); // read a new frame from video 

        //Breaking the while loop if the frames cannot be captured
        if (bSuccess == false) {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }

        //show the frame in the created window
        imshow(window_name, frame);
        
        //wait for for 10 ms until any key is pressed.  
        //If the 'Esc' key is pressed, break the while loop.
        //If the any other key is pressed, continue the loop 
        //If any key is not pressed withing 10 ms, continue the loop 
        if (waitKey(10) == 27){
                cout << "Esc key is pressed by user. Stoppig the video" << endl;
                break;
            }
    }*/
    return 0;
}