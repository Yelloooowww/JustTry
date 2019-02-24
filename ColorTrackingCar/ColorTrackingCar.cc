#include <opencv2/core/utility.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <ctype.h>

#include<wiringPi.h>
#include<softPwm.h>

#define INT1 4
#define INT2 5
#define INT3 6
#define INT4 26

#define Auto 0
#define manual 1

using namespace cv;
using namespace std;

Mat image;
bool selectObject = false;
int trackObject = 0;
Point origin;
Rect selection;
VideoCapture cap;
Rect trackWindow;
Mat frame, hsv, hue, mask, hist= Mat::zeros(200, 320, CV_8UC3), backproj;
char now_state=Auto;
int x_max=540,x_min=100;

void CarForward()
{
    digitalWrite(INT1,0);
    digitalWrite(INT2,1);
    digitalWrite(INT3,1);
    digitalWrite(INT4,0);
}
void CarStop()
{
    digitalWrite(INT1,1);
    digitalWrite(INT2,1);
    digitalWrite(INT3,1);
    digitalWrite(INT4,1);
}
void CarLeft()
{
    digitalWrite(INT1,0);
    digitalWrite(INT2,1);
    digitalWrite(INT3,0);
    digitalWrite(INT4,1);
}
void CarRight()
{
    digitalWrite(INT1,1);
    digitalWrite(INT2,0);
    digitalWrite(INT3,1);
    digitalWrite(INT4,0);
}

void SelfDrivingCar(RotatedRect p)
{
    if(p.size.height>=480*0.9 || p.size.width>=640*0.9 )
    {
        cout<<"Arrive~~~height="<< p.size.height<<"width="<<p.size.width<<endl;
    }
    else
    {
        cout<<"SelfDriving x="<<p.center.x<<" y="<<p.center.y;
        if(p.center.x<min(x_max,x_min))
        {
            CarLeft();
            cout<<" TurnLeft";
        }
        else if(p.center.x>max(x_max,x_min))
        {
            CarRight();
            cout<<" TurnRight";
        }
        else
        {
            CarForward();
            cout<<" Forward";
        }

        cout<<endl;
    }
}

void solveKeyboardInput(char c)
{
    switch(c)
    {
        case 'a':
            {
                if(now_state==manual)
                {
                    CarLeft();
                    cout<<"manual Left"<<endl;
                }
                break;
            }
        case 'd':
            {
                if(now_state==manual)
                {
                    CarRight();
                    cout<<"manual Right"<<endl;
                }
                break;
            }
        case 'w':
            {
                if(now_state==manual)
                {
                    CarForward();
                    cout<<"manual Forward"<<endl;
                }
                break;
            }
        case 's':
            {
                if(now_state==manual)
                {
                    CarStop();
                    cout<<"manual Stop"<<endl;
                }
                break;
            }
        case 'm':
            {
                if(now_state==Auto) now_state=manual;
                else now_state=Auto;

                break;
            }
        case 'n':
            {
                trackObject = 0;
                break;
            }
    }
}

// User draws box around object to track. This triggers CAMShift to start tracking
static void onMouse( int event, int x, int y, int, void* )
{
    if( selectObject )
    {
        selection.x = MIN(x, origin.x);
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);
        selection.height = std::abs(y - origin.y);

        selection &= Rect(0, 0, image.cols, image.rows);
    }

    switch( event )
    {
        case EVENT_LBUTTONDOWN:
            origin = Point(x,y);
            selection = Rect(x,y,0,0);
            selectObject = true;
            break;
        case EVENT_LBUTTONUP:
            selectObject = false;
            if( selection.width > 0 && selection.height > 0 )
                trackObject = -1;   // Set up CAMShift properties in main() loop
            break;
    }
}

RotatedRect CameraDetect()
{
    RotatedRect trackBox;
    int vmin = 0, vmax = 256, smin = 30;
    int hsize = 16;
    float hranges[] = {0,180};
    const float* phranges = hranges;

    cap >> frame;
    frame.copyTo(image);
    cvtColor(image, hsv, COLOR_BGR2HSV);//HSV(hue、saturation、value)用來表示色相、飽和度和明度

    if( trackObject )
    {
        int _vmin = vmin, _vmax = vmax;

        inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),Scalar(180, 256, MAX(_vmin, _vmax)), mask);
        //即检查数组元素是否在另外两个数组元素值之间。这里的数组通常也就是矩阵Mat或向量。要特别注意的是：该函数输出的dst是一幅二值化之后的图像。

        int ch[] = {0, 0};
        hue.create(hsv.size(), hsv.depth());
        mixChannels(&hsv, 1, &hue, 1, ch, 1);

        if( trackObject < 0 )
        {
            // Object has been selected by user, set up CAMShift search properties once
            Mat roi(hue, selection), maskroi(mask, selection);
            calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
            normalize(hist, hist, 0, 255, NORM_MINMAX);

            trackWindow = selection;
            trackObject = 1; // Don't set up again, unless user selects new ROI
        }

        // Perform CAMShift
        calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
        backproj &= mask;
        trackBox = CamShift(backproj, trackWindow,TermCriteria( TermCriteria::EPS | TermCriteria::COUNT, 10, 1 ));
        if( trackWindow.area() <= 1 )
        {
            int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
            trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,trackWindow.x + r, trackWindow.y + r) &Rect(0, 0, cols, rows);
        }

        ellipse( image, trackBox, Scalar(0,0,255), 3, LINE_AA );
    }

    if( selectObject && selection.width > 0 && selection.height > 0 )
    {
        Mat roi(image, selection);
        bitwise_not(roi, roi);
    }

    line(image, Point(x_max,0), Point(x_max,480), Scalar(255,0,0));
    line(image, Point(x_min,0), Point(x_min,480), Scalar(0,255,0));
    imshow( "Color Tracking Car", image );

    return trackBox;
}

int main( int argc, const char** argv )
{
    if(wiringPiSetup() == -1) return 0;
    pinMode(INT1,1);
    pinMode(INT2,1);
    pinMode(INT3,1);
    pinMode(INT4,1);
    CarStop();

    cap.open(0);
    if( !cap.isOpened() )
    {
        cout << "***Could not initialize capturing...***\n";
        return -1;
    }

    namedWindow( "Color Tracking Car", 0 );
    setMouseCallback( "Color Tracking Car", onMouse, 0 );
    createTrackbar( "Xmax", "Color Tracking Car", &x_max, 640, 0 );
    createTrackbar( "Xmin", "Color Tracking Car", &x_min, 640, 0 );

    while(1)
    {
        RotatedRect destination=CameraDetect();
        char c = (char)waitKey(1);
        if(c==27) break;
        solveKeyboardInput(c);
        if(now_state==Auto && trackObject==1) SelfDrivingCar(destination);
        delay(25);
        CarStop();

    }

    return 0;
}
