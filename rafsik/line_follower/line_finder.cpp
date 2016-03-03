#include "line_finder.hpp"

bool line_finder_debug_mode=false;

int findLineCenter(Mat& _input_img, Rect _scan_area, bool _draw_location )
{
    Mat img_scan;
    Mat img_tresh;

    int line_pos=-1;

    //needed by find contours algorithm
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    //copy _scan_area part of input image to img_scan variable
    _input_img(_scan_area).copyTo(img_scan);
    //switch scan area colors to grayscale
    cvtColor(img_scan, img_scan, CV_BGR2GRAY );

    if(line_finder_debug_mode){
        imshow("1. LINE_SCAN_AREA", img_scan);
    }

    //scan area binarization
    threshold( img_scan, img_tresh, 50, 180, THRESH_BINARY_INV );

    if(line_finder_debug_mode){
        imshow("2. LINE_SCAN_BIN", img_tresh);
    }

    //remove noisy elements by erode and dilate operations
    Mat erodeElmt = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat dilateElmt = getStructuringElement(MORPH_RECT, Size(5, 5));

    erode(img_tresh, img_tresh, erodeElmt);
    if(line_finder_debug_mode){
        imshow("3. ERODE_RESULT", img_tresh);
    }
    dilate(img_tresh, img_tresh, dilateElmt);
        if(line_finder_debug_mode){
        imshow("4. DILATE_RESULT", img_tresh);
    }

    //find contours
    findContours(img_tresh, contours, hierarchy, CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));

    for (size_t i = 0; i < contours.size(); i++)
    {
        //find the biggest contour
        float area = contourArea(contours[i]);
        if (area > MIN_LINE_CONTOUR_AREA)
        {
            //calculate contour center
            Moments mu;
            mu = moments(contours[i], false);
            Point2f center(mu.m10 / mu.m00, (_scan_area.height/2)+_scan_area.y); // point in center (x only)
            if(_draw_location){
                //circle(_input_img, center, 10, Scalar(0, 255, 0), 1, 8, 0);
                //rectangle(_input_img, Point(center.x-30, center.y+30), Point(center.x+30, center.y-30), Scalar( 0, 255, 0 ), 1, 8, 0);
                line(_input_img, Point(center.x-20, center.y), Point(center.x+20, center.y), Scalar( 0, 255, 0 ), 1, CV_AA, 0);
                line(_input_img, Point(center.x, center.y-20), Point(center.x, center.y+20), Scalar( 0, 255, 0 ), 1, CV_AA, 0);
            }
            line_pos=center.x;
        }
    }
    return line_pos;
}

void drawHorizontalArrow(Mat &img, Point start_p, int arrow_length, const Scalar& color, int line_thickness, int lineType)
{
     vector<Point> arrow_points;
     vector<Point>::iterator vertex;

     arrow_points.push_back(start_p);
     arrow_points.push_back(Point(start_p.x, start_p.y+arrow_length/4));
     arrow_points.push_back(Point(start_p.x+arrow_length/2, start_p.y+arrow_length/4));
     arrow_points.push_back(Point(start_p.x+arrow_length/2, start_p.y+arrow_length/2));
     arrow_points.push_back(Point(start_p.x+arrow_length, start_p.y));
     arrow_points.push_back(Point(start_p.x+arrow_length/2, start_p.y-arrow_length/2));
     arrow_points.push_back(Point(start_p.x+arrow_length/2, start_p.y-arrow_length/4));
     arrow_points.push_back(Point(start_p.x, start_p.y-arrow_length/4));
     arrow_points.push_back(start_p);

     for( vertex = arrow_points.begin(); vertex != (arrow_points.end())-1; vertex++){
        line(img, *vertex, *(vertex+1), color, line_thickness, lineType, 0);
     }

}
