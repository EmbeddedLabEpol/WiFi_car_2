#include "color_finder.hpp"

bool color_finder_debug_mode=false;

Mat colorThresholdCallibration(Mat &img)
{
    Mat hsv_img;
    Mat thresh_img;

    //defaults
    static int min_H = 15;
    static int max_H = 30;

    static int min_S = 130;
    static int max_S = 250;

    static int min_V = 90;
    static int max_V = 255;

    GaussianBlur(img, img, Size(9, 9), 2, 2); //Apply Blur

    //Create callibration window
    const char calib_win_name[]="Color threshold callibration";

    cvCreateTrackbar("min_H", calib_win_name, &min_H, 255); //Hue (0 - 255)
    cvCreateTrackbar("max_H", calib_win_name, &max_H, 255);

    cvCreateTrackbar("min_S", calib_win_name, &min_S, 255); //Saturation (0 - 255)
    cvCreateTrackbar("max_S", calib_win_name, &max_S, 255);

    cvCreateTrackbar("min_V", calib_win_name, &min_V, 255); //Value (0 - 255)
    cvCreateTrackbar("max_V", calib_win_name, &max_V, 255);

    // switch color palette to HSV
    cvtColor(img, hsv_img, CV_BGR2HSV);
    if(color_finder_debug_mode){
        imshow("1.HSV_CONV_RESULT", hsv_img);
    }

    // apply thresholding
    inRange(hsv_img, Scalar(min_H, min_S, min_V), Scalar(max_H, max_S, max_V), thresh_img);
    imshow(calib_win_name, thresh_img);

    //remove noisy elements by erode and dilate operations
    Mat erodeElmt = getStructuringElement(MORPH_RECT, Size(6, 6));
    Mat dilateElmt = getStructuringElement(MORPH_RECT, Size(3, 3));

    erode(thresh_img, thresh_img, erodeElmt);
    if(color_finder_debug_mode){
        imshow("2.ERODE_RESULT", thresh_img);
    }
    dilate(thresh_img, thresh_img, dilateElmt);
    if(color_finder_debug_mode){
        imshow("3.DILATE_RESULT", thresh_img);
    }

    return thresh_img;
}

Point findObjectMoments(Mat &img, Mat &thresh_img)
{
    Point obj_center=Point(-1,-1);

    Moments mu = moments(thresh_img, true);

    obj_center.x=mu.m10 / mu.m00;
    obj_center.y=mu.m01 / mu.m00;

    // Show info on the screen
    int info_sqr_size = countNonZero(thresh_img)/100;

    Scalar info_color= Scalar(255, 255, 255);

    rectangle(img, Point( obj_center.x-info_sqr_size/2, obj_center.y-info_sqr_size/2),
                   Point( obj_center.x+info_sqr_size/2, obj_center.y+info_sqr_size/2), info_color, 2, 4, 0);
    // Show additional info
    char circle_info[32];

    sprintf(circle_info, "OBJ x %d y %d", obj_center.x, obj_center.y);

    putText(img, circle_info,  Point( obj_center.x-info_sqr_size/2 , obj_center.y- info_sqr_size/2-5),
            CV_FONT_HERSHEY_SIMPLEX, 0.6, info_color, 1, CV_AA, false);

    return obj_center;
}

Point findObjectHough(Mat &img, Mat &thresh_img)
{
    Mat circ_img=::Mat::zeros(thresh_img.rows, thresh_img.cols, CV_8UC1);

    //vector of circle shapes found in img
    vector<Vec3f> circles; //circle data in Vec3f: v[0]=x v[1]=y v[2]=r
    vector<Vec3f>::const_iterator it= circles.begin();

    //find circles using HoughCircles Algorithm
    HoughCircles(thresh_img, circles, CV_HOUGH_GRADIENT,
                4,   // accumulator resolution (size of the image / 8)
                2000,  // minimum distance between two circles
                200, // Canny high threshold
                40, // minimum number of votes
                20, 500); // min and max circle radius

    //print no of circles found
    if(color_finder_debug_mode){
        cout << circles.size() << " circles found" << endl;
    }

    //find the biggest one
    int max_r=0;
    Vec3f big_circ=Vec3f(-1,-1,-1);

    for(it = circles.begin(); it != circles.end(); it++ ){
        if((*it)[2] > max_r){
            max_r=(*it)[2];
            big_circ= *it;
        }
    }
    //if found draw it
    if(max_r != 0){
        int d=big_circ[2]; //half of square edge
        Scalar info_color= Scalar(0, 255, 0);

        rectangle(img, Point( big_circ[0]-d, big_circ[1]-d), Point( big_circ[0]+d, big_circ[1]+d), info_color, 2, 4, 0);
        //circle(img, Point( big_circ[0], big_circ[1]), big_circ[2] , info_color, 2, CV_AA, 0);

        // Additional info
        char circle_info[32];
        sprintf(circle_info, "OBJ x %d y %d", (int)big_circ[0], (int)big_circ[1]);

        putText(img, circle_info,  Point( big_circ[0]-d, big_circ[1]-d-4),
                CV_FONT_HERSHEY_SIMPLEX, 0.6, info_color, 1, CV_AA, false);
    }
    return Point(big_circ[0],big_circ[1]);
}
