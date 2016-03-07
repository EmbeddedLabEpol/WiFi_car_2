#include <ctime>
#include <unistd.h>
#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>                        //Sposób
//dokładny
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
int main ( int argc,char **argv )
{
    cv::VideoCapture cap(0);
    //cap.set(CV_CAP_PROP_FRAME_WIDTH, 1024);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT, 768);
    cv::Mat img;
    cv::Mat img_gray;

    string face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";    //Nazwa kaskady którą wykorzystujemy do rozpoznania twarzy
    cv::CascadeClassifier face_cascade;
    vector<cv::Rect> faces;                            //Utworzenie bufora na twarze
    if( !face_cascade.load( face_cascade_name ) )        //Ładowanie pliku ze sprawdzeniem poprawnoci
    {
        cout << "Nie znaleziono pliku " << face_cascade_name << ".";
        return -2;
    }

    for ( int i =0 ; i <atoi(argv[1]) ; ++i ){
        cap >> img;
        //img = cv::Mat();
        img_gray = cv::Mat();
        std::cout << img.elemSize();
        std::cout <<  " kolejka " << i << " wielkosc " <<img.size() <<" sizeof "<<sizeof(img)<< std::endl;

        cv::cvtColor(img, img_gray,  CV_BGR2GRAY );                //Konwersja obrazu do odcieni szarosci
        //cout << " po konwersji "<<endl;
        faces.clear();
        //cout << " po czyszczeniu vektora "<<endl;

        face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, cv::Size(50, 50) );
        //cout << " po detekcji "<<endl;


        for( unsigned i = 0; i < faces.size(); i++ )
        {
            cv::Rect rect_face( faces[i] );    //Kwadrat okreslający arz
            //ellipse( img, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 120, 12 ), 2, 2, 0 );
            cv::rectangle(img, rect_face, cv::Scalar( 120, 5, 86 ), 2, 2, 0  );
           cout << "kwadrat jest x: " <<rect_face.x << " y: "<< rect_face.y <<endl;
           cout << " wysokosc: " <<rect_face.height << " szerkokosc: "<<rect_face.width <<endl;
        } //for faces.size()
        //cout << " po wpisaniu trojkatow  "<<endl;
        if (faces.size()>0){
            time_t data;
            time(&data);

            cv::putText(img, ctime(&data), cvPoint(30,30),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 1.8, cvScalar(200,200,250), 1, CV_AA);
            cv::imwrite("/mnt/ramdisk/raspicam_cv_image2.jpg",img);
            cout<<"Image saved at raspicam_cv_image.jpg"<<endl;
        } //if



    } ////for
    if (img.empty()){
        std::cout << " buba "<<std::endl;
    }
    cv::imwrite("test.jpg",img);
    return 0;
}

/*using namespace std;
string face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";    //Nazwa kaskady którą wykorzystujemy do rozpoznania twarzy 
cv::CascadeClassifier face_cascade;
vector<cv::Rect> faces;                            //Utworzenie bufora na twarze
//Utworzenie bufora na obrazek w odcieniach szarosci
double secondsElapsed ;

int main ( int argc,char **argv ) {

    time_t timer_begin,timer_end;
    raspicam::RaspiCam_Cv Camera;
    cv::Mat image,img;
    cv::Mat img_gray;
    int nCount=10;
    //set camera params
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC2 );
    //Open camera
    cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
    //Start capture
    cout<<"Capturing "<<nCount<<" frames ...."<<endl;
    if( !face_cascade.load( face_cascade_name ) )        //Ładowanie pliku ze sprawdzeniem poprawnoci
    {
        cout << "Nie znaleziono pliku " << face_cascade_name << ".";
        return -2;
    }
    for ( int u = 0; u<50000;++u){
        usleep(500000);
        time ( &timer_begin );
        //for ( int i=0; i<nCount; i++ ) {
        Camera.grab();
        Camera.retrieve ( image);
        cout<<"\r captured "<<" images  iteracja: "<<u<<" !"<<std::flush;
        //}
        //cout<<"Stop camera..."<<endl;

        //show time statistics
        time ( &timer_end ); // get current time; same as: timer = time(NULL)
        secondsElapsed = difftime ( timer_end,timer_begin );
        //cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<<  ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
        //save image
        //cout << " wielkosc mat image " << sizeof(cv::Mat) << endl;

        //cv::imwrite("/mnt/ramdisk/raspicam_cv_image1.jpg",image);
        cv::cvtColor(image, img_gray,  CV_BGR2GRAY );                //Konwersja obrazu do odcieni szarosci
        cout << " po konwersji "<<endl;
        faces.clear();
        cout << " po czyszczeniu vektora "<<endl;

        face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, cv::Size(50, 50) );
        cout << " po detekcji "<<endl;


        for( unsigned i = 0; i < faces.size(); i++ )
        {
            cv::Rect rect_face( faces[i] );    //Kwadrat okreslający twarz
            //ellipse( img, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 120, 12 ), 2, 2, 0 );
            cv::rectangle(img, rect_face, cv::Scalar( 120, 5, 86 ), 2, 2, 0  );
        }
        cout << " po wpisaniu trojkatow  "<<endl;
        if (faces.size()>0){
            time_t data;
            time(&data);

            cv::putText(image, ctime(&data), cvPoint(30,30),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 1.8, cvScalar(200,200,250), 1, CV_AA);
            cv::imwrite("/mnt/ramdisk/raspicam_cv_image2.jpg",image);
            cout<<"Image saved at raspicam_cv_image.jpg"<<endl;
        } //if
    }//for
    cv::putText(image, "Rafal  daj kod magisterki .", cvPoint(30,30),
                cv::FONT_HERSHEY_COMPLEX_SMALL, 1.8, cvScalar(200,200,250), 1, CV_AA);




    cv::imwrite("/mnt/ramdisk/raspicam_cv_image1.jpg",image);

}
*/
