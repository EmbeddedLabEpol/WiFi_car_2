#ifndef SYMBOL_FINDER_H_
#define SYMBOL_FINDER_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <dirent.h>
#include <stdio.h>

/* Define biggest difference in pixels between binarized symbol image
   and the camera image */
#define MAX_IMG_DIFF    5000

/* Define symbol images resolution */
#define REF_SIGN_IMG_X   195
#define REF_SIGN_IMG_Y   271

using namespace cv;
using namespace std;

// TYPEDEFS
/*  symbol definition:
    img - symbol image loaded from file
    name - symbol name identical as img filename */
typedef struct
{
    Mat     img;
	string name;
} symbol_t;

/*  rectangle definition:
    img - wraped image part of image seen by camera
    center - visible rectangle center
    area   - visible rectangle area in px^2 */
typedef struct
{
    Mat     img;
    Point center;
	float   area;
} rect_prop_t;

/*  set this true, to show all image processing steps */
extern bool symbol_finder_debug_on;

/* Symbol database class definition */
class SYMBOL_DB
{
    public:
        /* class constructor */
        SYMBOL_DB();
        /* returns symbol name */
        string getSymbolName(const int i) const;
        /* loads symbol database from given directory, returns 0 on success */
        int  loadSymbolDatabaseFromDirectory(const char* _img_dir);
        /* search matching symbol index in database, returns symbol
        index or -1 if not found */
        int  findMatchingSymbolIdx(Mat &_frame) const;
        /* returns symbol image pointer */
        const Mat* getSymbolImage(int _symbol_idx) const;
    protected:
    private:
        vector<symbol_t> symbol_database;
};

/* Function is searching rectangle shapes in image
/param _frame - input image
/param min_area - minimal area of rectangle in px^2
/param draw_location - function drawing rectangle center, and contours on input image
/return found rectangle data */
rect_prop_t findRectangle(Mat &_frame, int min_area, bool draw_location);

#endif // SYMBOL_FINDER_H_
