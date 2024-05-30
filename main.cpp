#include <windows.h>
#include <winuser.h>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

WORD LEFT_KEY = 'C';
WORD RIGHT_KEY = 'M';
const char gameWindowTitle[] = "Untitled - Notepad";

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

int main(int argc, char* argv[]){
    //setup inputs
    //keep seperate for ease of keeping track
    
    //write a function that takes a char and presses down a key

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
    */HWND gameWindow = FindWindow(NULL, gameWindowTitle);
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
    Sleep (5000);
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