#include "symbol_finder.hpp"

bool symbol_finder_debug_on=false;

SYMBOL_DB::SYMBOL_DB()
{

}

string SYMBOL_DB::getSymbolName(const int i) const
{
    return this->symbol_database[i].name;
}

int SYMBOL_DB::loadSymbolDatabaseFromDirectory(const char* _img_dir)
{
    string file_path;
    DIR *dir;
    struct dirent *new_file;
    symbol_t new_symbol;

    dir = opendir(_img_dir);
    if(dir != NULL)
    {
        printf("Loading symbol database from directory:\t%s\n",_img_dir);
        while((new_file = readdir(dir)) != NULL)
        {
            if(new_file->d_name[0] != '.'){
                file_path.assign(cv::format("%s/%s", _img_dir, new_file->d_name));
                new_symbol.img = imread(file_path, CV_LOAD_IMAGE_GRAYSCALE);
                threshold(new_symbol.img, new_symbol.img, 100, 255, 0);
                new_symbol.name = new_file->d_name;
                this->symbol_database.push_back(new_symbol);

                printf("\tImage: %s \tloaded\n", new_file->d_name);
            }
        }
        closedir(dir);
    } else {
        /* could not open directory */
        perror ("img_database directory not found");
        return 1;
    }
	return 0;
}

const Mat* SYMBOL_DB::getSymbolImage(int _symbol_idx) const
{
    return &(this->symbol_database[_symbol_idx].img);
}

static void sortCorners(vector<cv::Point2f>& corners, cv::Point2f center)
{
	vector<cv::Point2f> top, bot;

	for (unsigned int i = 0; i < corners.size(); i++) {
		if (corners[i].y < center.y)
			top.push_back(corners[i]);
		else
			bot.push_back(corners[i]);
	}

	Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
	Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
	Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
	Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];

	corners.clear();
	corners.push_back(tl);
	corners.push_back(tr);
	corners.push_back(br);
	corners.push_back(bl);
}

rect_prop_t findRectangle(Mat &_frame, int min_area, bool draw_location)
{
    rect_prop_t rect_data;
    rect_data.area=0;
    rect_data.center.x=-1;
    rect_data.center.y=-1;

    Mat grey_img;
    cvtColor(_frame, grey_img, CV_RGB2GRAY);   //switch colors to BW
    if(symbol_finder_debug_on){
        imshow("1.GREY_IMG", grey_img);
    }

    Mat canny_input;
    Mat canny_img;
    GaussianBlur(grey_img, canny_input, Size(9, 9), 2, 2); //Apply Blur
    if(symbol_finder_debug_on){
        imshow("2.GAUSSIAN_BLUR_IMG", grey_img);
    }
    Canny(canny_input, canny_img, 50, 100, 3); //Use Canny edge detection
    if(symbol_finder_debug_on){
        imshow("3.CANNY_IMG", canny_img);
    }

    //FindContours
    vector<vector<Point> > contours;    //vector of vectors
    vector<Vec4i> hierarchy;            //needed by findcontours algorithm

    findContours(canny_img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    //aproximated rectangle
    vector<Point> approx_shape;
    Point center(-1,-1);
    // Define the destination rectangle image
    Mat warped_img = ::Mat::zeros(REF_SIGN_IMG_X, REF_SIGN_IMG_Y, CV_8UC1);
    // Corners of the destination image
    vector<cv::Point2f> quad_pts;
    quad_pts.push_back(Point2f(0, 0));
    quad_pts.push_back(Point2f(warped_img.cols, 0));
    quad_pts.push_back(Point2f(warped_img.cols, warped_img.rows));
    quad_pts.push_back(Point2f(0, warped_img.rows));

    for(size_t i = 0; i < contours.size(); i++)
    {
        // domykanie konturow poprzez aproksymacje krzywymi
        approxPolyDP(contours[i], approx_shape, arcLength(Mat(contours[i]), true) * 0.1, true);
        if (approx_shape.size() == 4) //Poszukiwanie wsrod konturow 4-katow
        {
            float area = contourArea(contours[i]);
            if (area > min_area)
            {
                std::vector<cv::Point2f> corners;
                vector<Point>::iterator vertex;

                for(vertex = approx_shape.begin(); vertex != approx_shape.end(); ++vertex )
                {
                    corners.push_back(*vertex);
                    if(draw_location == true){
                        //draw circles on corners
                        //circle(_frame, *vertex, 5, Scalar(10, 255, 10), 1, CV_AA, 0);
                        //or draw lines between corners
                        Scalar line_col = Scalar(10, 255, 10);
                        line(_frame, approx_shape[0], approx_shape[1], line_col, 1, CV_AA, 0);
                        line(_frame, approx_shape[1], approx_shape[2], line_col, 1, CV_AA, 0);
                        line(_frame, approx_shape[2], approx_shape[3], line_col, 1, CV_AA, 0);
                        line(_frame, approx_shape[3], approx_shape[0], line_col, 1, CV_AA, 0);
                    }
                }

                //Find Center
                Moments mu;
                mu = moments(contours[i], false);
                center.x=mu.m10 / mu.m00;
                center.y=mu.m01 / mu.m00;
                if(draw_location == true){
                    //draw circle at center
                    circle(_frame, center, 5, Scalar(0, 0, 255), 1, CV_AA, 0);
                }

                //Sort corners around center
                sortCorners(corners, center);

                // Get transformation matrix
                Mat trans_mtx = getPerspectiveTransform(corners, quad_pts);

                // Apply perspective transformation
                warpPerspective(grey_img, warped_img, trans_mtx, warped_img.size());
                if(symbol_finder_debug_on){
                    imshow("4.WARPED RECT IMG", warped_img);
                }
                rect_data.area=area;
                rect_data.center=center;
            }
        }
    }
    rect_data.img=warped_img;
    return rect_data;
}

int SYMBOL_DB::findMatchingSymbolIdx(Mat &_frame) const
{
    Mat bin_img;
    Mat diff_img;
    int diff;
    int match_idx=-1;

    double min_val,max_val,mid_val, thresh_val;

    minMaxLoc(_frame, &min_val, &max_val);
    mid_val=(max_val-min_val)/2;
    thresh_val=1.5*mid_val+min_val;

    threshold(_frame, bin_img, thresh_val, 255, 0);
    if(symbol_finder_debug_on){
        imshow("BIN IMG", bin_img);
    }

    for (int i = 0; i < this->symbol_database.size(); i++)
    {
        bitwise_xor(bin_img, this->symbol_database[i].img, diff_img, noArray());
        diff = countNonZero(diff_img);
        if (diff < MAX_IMG_DIFF) {
            match_idx = i;
            break;
        }
    }
    return match_idx;
}
