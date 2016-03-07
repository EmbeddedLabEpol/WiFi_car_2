#include "camera.hpp"
using namespace std;
void * camera_thread (  void * data  )
{
	pthread_detach( pthread_self () );
	thread_data  *my_data;
    my_data = (thread_data*)data;
    cv::VideoCapture cap(0);
    //cap.set(CV_CAP_PROP_FRAME_WIDTH, 1024);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT, 768);
    cv::Mat img;
    cv::Mat img_gray;

    string face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";    //Nazwa kaskady kt�r wykorzystujemy do rozpoznania twarzy
    cv::CascadeClassifier face_cascade;
    vector<cv::Rect> faces;                            //Utworzenie bufora na twarze
    if( !face_cascade.load( face_cascade_name ) )        //Aadowanie pliku ze sprawdzeniem poprawnoci
    {
        cout << "Nie znaleziono pliku " << face_cascade_name << ".";
        //return -2;
    }

    for ( int i =0 ; i <120 ; ++i ){
        cap >> img;
        //img = cv::Mat();
        img_gray = cv::Mat();
        //std::cout << img.elemSize();
        //std::cout <<  " kolejka " << i << " wielkosc " <<img.size() <<" sizeof "<<sizeof(img)<< std::endl;

        cv::cvtColor(img, img_gray,  CV_BGR2GRAY );                //Konwersja obrazu do odcieni szarosci
        //cout << " po konwersji "<<endl;
        faces.clear();
        //cout << " po czyszczeniu vektora "<<endl;

        face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, cv::Size(50, 50) );
        //cout << " po detekcji "<<endl;


        for( unsigned i = 0; i < faces.size(); i++ )
        {
            cv::Rect rect_face( faces[i] );    //Kwadrat okreslajcy arz
            //ellipse( img, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 120, 12 ), 2, 2, 0 );
            cv::rectangle(img, rect_face, cv::Scalar( 120, 5, 86 ), 2, 2, 0  );
            cout << "\r kwadrat jest x: " <<rect_face.x << " y: "<< rect_face.y
             << " wysokosc: " <<rect_face.height << " szerkokosc: "<<rect_face.width <<std::flush;

        } //for faces.size()
        //cout << " po wpisaniu trojkatow  "<<endl;
        if (faces.size()>0){

            time_t data;
            time(&data);

            cv::putText(img, ctime(&data), cvPoint(30,30),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 1.8, cvScalar(200,200,250), 1, CV_AA);
            cv::imwrite("/mnt/ramdisk/raspicam_cv_image2.jpg",img);
           // cout<<"Image saved at raspicam_cv_image.jpg"<<endl;
        } //if



    } ////for
    if (img.empty()){
        std::cout << " buba "<<std::endl;
    }
    //cv::imwrite("test.jpg",img);
	 for (int i =0 ; i< MAX_CONNECTION;++i)
    {

        if (my_data->main_THREAD_arr[i].thread_ID == pthread_self())
        {
            my_data->main_THREAD_arr[i].thread_ID = 0;
            my_data->main_THREAD_arr[i].thread_name ="  -empty-  ";

            break;
        }
    }
    pthread_exit(NULL);

}


