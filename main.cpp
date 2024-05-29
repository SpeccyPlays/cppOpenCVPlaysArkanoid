//Uncomment the following line if you are compiling this code in Visual Studio
//#include "stdafx.h"

#include <windows.h>
#include <winuser.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]){
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
    */HWND hActiveWindow = FindWindow(NULL, "Untitled - Notepad");
    cout << hActiveWindow << endl;
    SetForegroundWindow(hActiveWindow);
    SetFocus(hActiveWindow);
    INPUT inputs[6] = {};
    ZeroMemory(inputs, sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 'B';
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'B';
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'E';
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = 'E';
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[4].type = INPUT_KEYBOARD;
    inputs[4].ki.wVk = 'N';
    inputs[5].type = INPUT_KEYBOARD;
    inputs[5].ki.wVk = 'N';
    inputs[5].ki.dwFlags = KEYEVENTF_KEYUP;
    for (int i = 0; i < 20; i++){
        UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
        if (uSent != ARRAYSIZE(inputs)){
            cout << ("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError())) << endl;
        }
    }

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