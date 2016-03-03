#ifndef COLOR_FINDER_H_
#define COLOR_FINDER_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "stdio.h"

extern bool color_finder_debug_mode;

using namespace cv;
using namespace std;

/* This function shows HSV values callibration window for color thresholding
/param img - input image
/return image after thresholding */
Mat   colorThresholdCallibration(Mat &img);

/* Tracking object on thresholdesd image by using HoughCircles algorithm
to find circle-like shapes on a binary image
/param img - input image
/param threshold image - same image after color thresholdimg
/return object position */
Point findObjectHough(Mat &img, Mat &thresh_img);

/* Tracking object on thresholdesd image by using Moments
to find center of shapes on a binary image
/param img - input image
/param threshold image - same image after color thresholdimg
/return object position */
Point findObjectMoments(Mat &img, Mat &thresh_img);

#endif // COLOR_FINDER_H_
