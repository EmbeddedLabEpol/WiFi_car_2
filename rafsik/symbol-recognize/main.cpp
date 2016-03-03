#include <opencv/highgui.h>
#include <iostream>
#include <unistd.h>
#include "config.h"
#ifdef USE_RASPICAM
#include <raspicam/raspicam_cv.h>
#endif // USE_RASPICAM
#include "symbol_finder.hpp"

using namespace std;
using namespace cv;

//global variables
static bool debug_mode_enabled=false;
static bool exit_program=false;

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
SYMBOL_DB g_symbol_db;
Mat g_cam_view;

//thread function declarations
void* cam_get_img(void *arg);


static void checkInputParameters(int argc, char * argv[])
{
    for(int i=0; i < argc; i++)
    {
        if(! strcmp(argv[i], "-debug")){
            cout << "DEBUG_MODE activated" << endl;
            debug_mode_enabled=true;
            symbol_finder_debug_on=true;
        }
        else {
            cout << "Optional program parameters:" << endl << endl
                 << "-debug to show all image processing steps" << endl
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
    rect_prop_t rectangle;
    int matching_symbol_idx;

    checkInputParameters(argc, argv);
    g_symbol_db.loadSymbolDatabaseFromDirectory(SYMBOL_DATABASE_DIR);
    start_cam_thread();

    while(!exit_program)
    {
        key = cvWaitKey(10);
        //hit ESC to quit application
        if(key == 0x1B){
            exit_program=true;
        }
        //lock camera
        pthread_mutex_lock(&cam_mutex);

        //find rectangle
        rectangle=findRectangle(g_cam_view, MIN_RECT_AREA, true );

        // Rectangle shape found on the camera image
        if(rectangle.area != 0 && debug_mode_enabled)
        {
            cout << endl << "rectangle visible at\t x " << rectangle.center.x
            << " y " << rectangle.center.y ;
        }

        // Find matching image in image database
        matching_symbol_idx=g_symbol_db.findMatchingSymbolIdx(rectangle.img);
        if(matching_symbol_idx != -1 )
        {
            // get symbol image, and show it in new window
            const Mat* symbol_img = g_symbol_db.getSymbolImage(matching_symbol_idx);
            string symbol_name = g_symbol_db.getSymbolName(matching_symbol_idx);
            string info = "Symbol found: ";
            info.append(symbol_name);

            imshow("Detected symbol", *symbol_img);
            //Put recognized image name on camera image
            putText(g_cam_view, info, cvPoint(10,30), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 1.5, CV_AA, false);

            cout << "\tSymbol type " <<  symbol_name << " recognized!" << endl;
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
