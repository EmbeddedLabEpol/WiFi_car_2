﻿#include <ctime>
#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>                        //Sposób
                   //dokładny
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
string face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";    //Nazwa kaskady którą wykorzystujemy do rozpoznania twarzy 
cv::CascadeClassifier face_cascade;
void detectFace( cv::Mat img )
{
    vector<cv::Rect> faces;                            //Utworzenie bufora na twarze
    cv::Mat img_gray;                                //Utworzenie bufora na obrazek w odcieniach szarosci

    cv::cvtColor(img, img_gray,  CV_BGR2GRAY );                //Konwersja obrazu do odcieni szarosci
    //equalizeHist(img_gray, img_gray);                    //Zaaplikowanie do obrazu jego historogramu, poprawa kontrastu
  cout << "START"<<endl;
    face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, cv::Size(50, 50) );
    cout << "STOP" <<endl;
    for( unsigned i = 0; i < faces.size(); i++ )
    {
        cv::Rect rect_face( faces[i] );    //Kwadrat okreslający twarz
        //ellipse( img, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 120, 12 ), 2, 2, 0 );
        cv::rectangle(img, rect_face, cv::Scalar( 120, 5, 86 ), 2, 2, 0  );
    }
    cv::imwrite("/mnt/ramdisk/raspicam_cv_image2.jpg",img);//ie o nazwie "Hello Face !"
}
int main ( int argc,char **argv ) {

   time_t timer_begin,timer_end;
   raspicam::RaspiCam_Cv Camera;
   cv::Mat image;
   int nCount=10;
   //set camera params
   Camera.set( CV_CAP_PROP_FORMAT, CV_8UC2 );
   //Open camera
   cout<<"Opening Camera..."<<endl;
   if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
   //Start capture
   cout<<"Capturing "<<nCount<<" frames ...."<<endl;
   time ( &timer_begin );
   for ( int i=0; i<nCount; i++ ) {
       Camera.grab();
       Camera.retrieve ( image);
       if ( i%5==0 )  cout<<"\r captured "<<i<<" images"<<std::flush;
   }
   cout<<"Stop camera..."<<endl;
   Camera.release();
   //show time statistics
   time ( &timer_end ); /* get current time; same as: timer = time(NULL)  */
   double secondsElapsed = difftime ( timer_end,timer_begin );
   cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<<  ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
   //save image
   cout << " wielkosc mat image " << sizeof(cv::Mat) << endl;

   if( !face_cascade.load( face_cascade_name ) )        //Ładowanie pliku ze sprawdzeniem poprawnoci
      {
          cout << "Nie znaleziono pliku " << face_cascade_name << ".";
          return -2;
      }
  detectFace(image);
    cv::putText(image, "Rafal  daj kod magisterki .", cvPoint(30,30),
    cv::FONT_HERSHEY_COMPLEX_SMALL, 1.8, cvScalar(200,200,250), 1, CV_AA);




    cv::imwrite("/mnt/ramdisk/raspicam_cv_image1.jpg",image);
   cout<<"Image saved at raspicam_cv_image.jpg"<<endl;
}
