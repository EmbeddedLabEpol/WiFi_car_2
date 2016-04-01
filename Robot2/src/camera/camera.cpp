#include "camera.hpp"
#include "../c_connection/c_connection.h"
using namespace std;
void * camera_thread (  void * data  )
{
    pthread_detach( pthread_self () );
    thread_data  *my_data;
    my_data = (thread_data*)data;

    //////////////// polaczenie
    struct sockaddr_in server;
    int v_socket;

    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(my_data->server_settings->PORT.c_str()) +1  ) ;
    if( inet_pton( AF_INET, my_data->server_settings->SERVER_IP.c_str(), & server.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( - 1 );
    }

    if(( v_socket = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        perror( "socket() ERROR" );
        exit( - 1 );
    }

    if( fcntl( v_socket, F_SETFL, O_NONBLOCK ) < 0 ) // fcntl()
    {
        perror( "fcntl() ERROR" );
        exit( - 1 );
    }
    // zgub wkurzający komunikat błędu "address already in use"
    int yes =1;
    if( setsockopt( v_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( int ) ) == - 1 ) {
        perror( "setsockopt" );
        exit( 1 );
    }
    socklen_t len = sizeof( server );
    if( bind( v_socket,( struct sockaddr * ) & server, len ) < 0 )
    {

        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "BIND problem: " <<  strerror(  errno )<< std::endl;
        log_file_mutex.mutex_unlock();
        perror( "bind() ERROR" );
        exit( - 1 );
    }

    if( listen( v_socket, MAX_CONNECTION ) < 0 )
    {
        log_file_mutex.mutex_lock();
        log_file_cout << CRITICAL << "Listen problem: " <<  strerror(  errno )<< std::endl;
        log_file_mutex.mutex_unlock();
        perror( "listen() ERROR" );
        exit( - 1 );
    }

    struct sockaddr_in from;
    int v_sock_ind = 0;

    ///////////////////////////////////////////////////// WHILE ////////////////////////////////////////////////////

    while (1)
    {

        bzero( & from, sizeof( from ) );
        if (!go_while) {break;}

        usleep(100000);

        if(( v_sock_ind = accept( v_socket,( struct sockaddr * ) & from, & len ) ) < 0 )
        {
            continue;
        }
        else
        {  my_data->s_client_sock =v_sock_ind;
            my_data->from=from;
            break;
        }
    }
    C_connection *client = new C_connection( my_data);

    log_file_mutex.mutex_lock();
    log_file_cout << INFO <<"polaczenie WIDEO  "   <<std::endl;
    log_file_mutex.mutex_unlock();
    /// //////////////////////////// koniec polacznie   teraz obrazek

    std::vector<unsigned char> data_buffer;
    std::vector<int> compression_params (cv::IMWRITE_JPEG_QUALITY,20);
    //compression_params.insert(1);
    //compression_params.insert(90);

    cv::VideoCapture cap(0);
    //cap.set(CV_CAP_PROP_FRAME_WIDTH, 800);
    //cap.set(CV_CAP_PROP_FRAME_HEIGHT, 600);
    cv::Mat img;
    cv::Mat img_gray;
    time_t date;
    string face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";    //Nazwa kaskady kt�r wykorzystujemy do rozpoznania twarzy
    cv::CascadeClassifier face_cascade;
    vector<cv::Rect> faces;                            //Utworzenie bufora na twarze
    if( !face_cascade.load( face_cascade_name ) )        //Aadowanie pliku ze sprawdzeniem poprawnoci
    {
        cout << "Nie znaleziono pliku " << face_cascade_name << ".";
        //return -2;
    }


    double start = 0;

    std::string temp_str = "START";
    std::string temp_str2 = "START";

    while (go_while){

        start= millis();
        img = cv::Mat();
        cap >> img;
        // cv::resize(img, img, cv::Size(320, 240), 0, 0, cv::INTER_CUBIC);
        img_gray = cv::Mat();
        //std::cout << img.elemSize();
        //std::cout <<  " kolejka " << i << " wielkosc " <<img.size() <<" sizeof "<<sizeof(img)<< std::endl;


        faces.clear();
        if (my_data->robot1.face_detection  == true){
            cv::cvtColor(img, img_gray,  CV_BGR2GRAY );
            face_cascade.detectMultiScale(img_gray, faces, 1.1, 3, 0|CV_HAAR_SCALE_IMAGE, cv::Size(50, 50) );



            for( unsigned i = 0; i < faces.size(); i++ )
            {
                cv::Rect *rect_face= new cv::Rect( faces[i] );    //Kwadrat okreslajcy arz
                //ellipse( img, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 120, 12 ), 2, 2, 0 );
                cv::rectangle(img, *rect_face, cv::Scalar( 120, 5, 86 ), 2, 2, 0  );
                temp_str2="  x: " + intToStr( rect_face->x +(rect_face->height/2) )+" y: "+ intToStr(rect_face->y+(rect_face->width/2))+  " "+ intToStr(rect_face->width);
                delete rect_face;
                if ( rect_face->width < 85 )
                {
                 send_to_arduino(my_data, "forward:180;" ) ;
                 }
               if ( rect_face->width > 105 ){
                send_to_arduino(my_data, "back:180;" ) ;
                }

            } //for faces.size()
            //cout << " po wpisaniu trojkatow  "<<endl;
            if (faces.size()>0){
                cv::putText(img, temp_str2, cvPoint(30,60),
                            cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);

            }

        }
        time(&date);

        cv::putText(img, temp_str, cvPoint(30,30),
                    cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);

        cv::imwrite("/mnt/ramdisk/raspicam_cv_image2.jpg",img);


        client->l_send_jpg("/mnt/ramdisk/raspicam_cv_image2.jpg");


        // cv::imencode(".jpg", img, data_buffer, compression_params);
        //client->l_send_jpg_buf(data_buffer);
        // Calculate frames per second

        temp_str = ctime(&date);
        temp_str += "  ";
        temp_str += intToStr(1000/(millis()-start));
        temp_str += "FPS";
    } ////for

    for (int i =0 ; i< MAX_CONNECTION;++i)
    {

        if (my_data->main_THREAD_arr[i].thread_ID == pthread_self())
        {
            my_data->main_THREAD_arr[i].thread_ID = 0;
            my_data->main_THREAD_arr[i].thread_name ="  -empty-  ";

            break;
        }
    }
    sleep (3);
    delete client;
    pthread_exit(NULL);

}


