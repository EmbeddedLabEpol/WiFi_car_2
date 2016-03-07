#include <ctime>
#include <unistd.h>
#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>                        //Spos�b
//dok�adny
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "../Robot2.h"
void * camera_thread (void *data );
