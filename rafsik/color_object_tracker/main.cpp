#include <opencv/highgui.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include "config.h"
#ifdef USE_RASPICAM
#include <raspicam/raspicam_cv.h>
#endif // USE_RASPICAM
#include "color_finder.hpp"

using namespace std;
using namespace cv;

typedef enum
{
    METHOD_HOUGH   =0x00,
    METHOD_MOMENTS =0x01
} tracking_method_t;

//global variables
static bool exit_program=false;
static tracking_method_t track_mthd= METHOD_HOUGH;

#ifdef USE_RASPICAM
raspicam::RaspiCam_Cv raspi_cam;
#else
CvCapture *usb_cam = cvCaptureFromCAM(1);
#endif

//threads
static pthread_t cam_thread;

//mutex
static pthread_mutex_t cam_mutex;

//global variables
Mat g_cam_view;

//thread function declarations
void* cam_get_img(void *arg);

static void checkInputParameters(int argc, char * argv[])
{
    for(int i=0; i < argc; i++)
    {
        if(! strcmp(argv[i], "-debug")){
            cout << "DEBUG_MODE activated" << endl;
            color_finder_debug_mode=true;
        }
        else if(! strcmp(argv[i], "-m")){
            track_mthd=METHOD_MOMENTS;
            cout << "using CV moments to track object" << endl;
        }
        else if(! strcmp(argv[i], "-h")){
            track_mthd=METHOD_HOUGH;
            cout << "using HoughCircles algorithm to track object" << endl;
        }
        else {
            cout << "Optional program parameters:" << endl << endl
                 << "-debug to show all image processing steps" << endl
                 << "-m to use CV moments to track object" << endl
                 << "-h (default)to use HoughCircles algorithm to track object" << endl
                 << endl;
        }
    }
}


static void start_cam_thread(void)
{
    //configure camera
    #ifdef USE_RASPICAM
    raspi_cam.set( CV_CAP_PROP_FORMAT, CV_8UC3 );
    raspi_cam.set( CV_CAP_PROP_FRAME_WIDTH, CAM_X_RES );
    raspi_cam.set( CV_CAP_PROP_FRAME_HEIGHT, CAM_Y_RES );

    if ( !raspi_cam.open()) {
		cerr<<"Error opening raspicam"<<endl;
    }
    raspi_cam.grab();
    raspi_cam.retrieve(g_cam_view);
    #else
    cvSetCaptureProperty( usb_cam, CV_CAP_PROP_FRAME_WIDTH, CAM_X_RES );
    cvSetCaptureProperty( usb_cam, CV_CAP_PROP_FRAME_HEIGHT, CAM_Y_RES );
    g_cam_view = cvQueryFrame(usb_cam);
    #endif // USE_RASPICAM

    int err;
    err = pthread_create(&cam_thread, NULL, &cam_get_img, NULL);
    if (err != 0){
        printf("\ncan't create camera capture thread");
    }
}

static void stop_cam_thread(void)
{
    pthread_join(cam_thread, NULL);
    pthread_mutex_destroy(&cam_mutex);
    #ifdef USE_RASPICAM
    raspi_cam.release();
    #else
    cvReleaseCapture(&usb_cam);
    #endif // USE_RASPICAM
}

//CAMERA THREAD
void* cam_get_img(void *arg)
{
    while(!exit_program)
    {
        pthread_mutex_lock(&cam_mutex);
        //capture new image
        #ifdef USE_RASPICAM
          raspi_cam.grab();
          raspi_cam.retrieve(g_cam_view);
        #else
          g_cam_view = cvQueryFrame(usb_cam);
        #endif // USE_RASPICAM

        pthread_mutex_unlock(&cam_mutex);
        usleep(CAM_UPDATE_T*1000);
    }
}

int main( int argc, char * argv[] )
{
    char key;
    Mat threshold_img;
    Rect scan_area= Rect(0, 3*CAM_Y_RES/4, CAM_X_RES, CAM_Y_RES/4);

    checkInputParameters(argc, argv);
    start_cam_thread();

    const string mthd_info_mm="using CV moments";
    const string mthd_info_mh="using HoughCircles algorithm";

    while(!exit_program)
    {
        key = cvWaitKey(2);
        //hit ESC to quit application
        if(key == 0x1B){
            exit_program=true;
        }
        //lock camera
        pthread_mutex_lock(&cam_mutex);

        //thresholding and color callibration
        threshold_img=colorThresholdCallibration(g_cam_view);

        //tracking
        if(track_mthd == METHOD_HOUGH){
            findObjectHough(g_cam_view, threshold_img);
            // print used method type on the screen
            putText(g_cam_view, mthd_info_mh,  Point(10,40), CV_FONT_HERSHEY_SIMPLEX,
                    0.7, Scalar(255,255,255), 1, CV_AA, false);

        }
        else {
            findObjectMoments(g_cam_view, threshold_img);
            // print used method type on the screen
            putText(g_cam_view, mthd_info_mm,  Point(10,40), CV_FONT_HERSHEY_SIMPLEX,
                    0.7, Scalar(255,255,255), 1, CV_AA, false);
        }
        // Show image from camera
        imshow("Camera view", g_cam_view);
        //free camera
        pthread_mutex_unlock(&cam_mutex);
    }
    // Clean up
    cvDestroyAllWindows();
    stop_cam_thread();
    return 0;
}
