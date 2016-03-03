#ifndef LINE_FINDER_H_
#define LINE_FINDER_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

extern bool line_finder_debug_mode;

using namespace cv;
using namespace std;

#define MIN_LINE_CONTOUR_AREA   4000

int findLineCenter(Mat& _input_img, Rect _scan_area, bool _draw_location );
void drawHorizontalArrow(Mat &img, Point start_p, int arrow_length, const Scalar& color, int line_thickness, int lineType);

#endif // LINE_FINDER_H_
