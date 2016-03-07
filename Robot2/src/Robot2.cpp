//#include "serialib/serialib.h"              //brak

#include "functions/functions.h"            // brak

#include "parser/parser.hpp"

#include <wiringPi.h>

std::string  _logfile  = "/tmp/robot_log.log";
std::string buffer ;
Logger log_file_mutex(_logfile);
//int max_msg = MAX_MSG_LEN*sizeof(int32_t);
bool go_while = true;

//////////// watek wysylajacy/obdbierajacy dane z portu RS232 ////////
void *Send_Recieve_rs232_thread (void *przekaz){
    thread_data_rs232 *data_rs232;
    data_rs232 = (thread_data_rs232*)przekaz;
    SerialPi serial_ardu(strdup( data_rs232->portRS232.c_str()));
    serial_ardu.begin( atoi( data_rs232->BaudRate.c_str()));
    std::string znak;

    log_file_mutex.mutex_lock();
    log_file_cout << INFO <<"otwarcie portu RS232 " <<  data_rs232->portRS232 << data_rs232->BaudRate<<std::endl;
    log_file_mutex.mutex_unlock();

    while (go_while)
    {
        //usleep(50);
        pthread_mutex_lock(&C_connection::mutex_who);
        if (data_rs232->pointer.ptr_who[0] == RS232)
        {
            pthread_mutex_lock(&C_connection::mutex_buf);

            data_rs232->pointer.ptr_who[0] = data_rs232->pointer.ptr_who[1];
            data_rs232->pointer.ptr_who[1]= RS232;
            serial_ardu.print(buffer.c_str());

            buffer.erase();

            while ( go_while){
                if (serial_ardu.available()>0){
                    buffer+=serial_ardu.read();
                }
                if (buffer[buffer.size()-1] == ';')
                {
                    buffer.erase(buffer.end()-1);
                    break;
                }


            }
            pthread_mutex_unlock(&C_connection::mutex_buf);
        }
        pthread_mutex_unlock(&C_connection::mutex_who);
    }

    pthread_exit(NULL);
}

///////////  watek wymiany polaczenia /////////////////////

void *Server_connectivity_thread(void *przekaz){
    thread_data  *my_data;
    my_data = (thread_data*)przekaz;

    pthread_detach( pthread_self () );

    C_connection *client = new C_connection( my_data);

    log_file_mutex.mutex_lock();
    log_file_cout << INFO <<"polaczenie z adresu  " <<  inet_ntoa( my_data->from.sin_addr)   <<std::endl;
    log_file_mutex.mutex_unlock();


    while (go_while)
    {
        if( client->c_recv(0) == -1 )
        {
            break;
        }

        // ###########################  analia wiadomoscu ####################################//
        if ( client->c_analyse() == false )   // stop runing idom_server
        {
            client->c_send("\nEND.\n");

            go_while = false;
            break;
        }

        // ###############################  koniec analizy   wysylanie wyniku do RS232 lub  TCP ########################

        if( client->c_send(0 ) == -1 )
        {
            perror( "send() ERROR" );
            break;
        }
    }
    sleep (3);
    delete client;
    pthread_exit(NULL);

}

int main()
{
    pthread_mutex_init(&C_connection::mutex_buf, NULL);
    pthread_mutex_init(&C_connection::mutex_who, NULL);
    pthread_mutex_init(&blockQueue::mutex_queue_char, NULL);
    pthread_mutex_init(&Logger::mutex_log, NULL);
    //pthread_t  main_th;
    //pthread_create (&main_th, NULL,&main_thread,NULL);

    config server_settings   =  read_config ( "/home/pi/cpp/Robot2/robot_config.cfg"    );     // strukruta z informacjami z pliku konfigu

    struct sockaddr_in server;
    int v_socket;

    thread_data node_data; // przekazywanie do watku

    time(&node_data.start);

    Thread_array_struc thread_array[MAX_CONNECTION];
    for (int i =0 ; i< MAX_CONNECTION;++i){
        thread_array[i].thread_name="  -empty-  ";
        thread_array[i].thread_ID=0;
        thread_array[i].thread_socket=0;
    }


    unsigned int who[2]={FREE, FREE};
    //int32_t bufor[ MAX_MSG_LEN ];


    ///////////////////////////////////////////  zaczynam wpisy do logu ////////////////////////////////////////////////////////////
    log_file_mutex.mutex_lock();
    log_file_cout << "\n*****************************************************************\n*****************************************************************\n  "<<  " \t\t\t\t\t start programu " << std::endl;
    log_file_cout << INFO  << "ID serwera\t"<< server_settings.ID_server << std::endl;
    log_file_cout << INFO  << "PortRS232\t"<< server_settings.portRS232 << std::endl;
    log_file_cout << INFO  << "BaudRate RS232\t"<< server_settings.BaudRate << std::endl;
    log_file_cout << INFO  << "port TCP \t"<< server_settings.PORT << std::endl;
    log_file_cout << INFO  << "serwer ip \t"<< server_settings.SERVER_IP  <<std::endl;
    log_file_cout << INFO  << "dodatkowe NODY w sieci:\n"  <<std::endl;
     log_file_mutex.mutex_unlock();


    ///////////////////////////////////////////////  koniec logowania do poliku  ///////////////////////////////////////////////////



    /////////////////////////////////////////////////   wypelniam  struktury przesylane do watkow  ////////////////////////
    thread_data_rs232 data_rs232;
    data_rs232.BaudRate=server_settings.BaudRate;
    data_rs232.portRS232=server_settings.portRS232;
    //data_rs232.pointer.ptr_buf=bufor;
    data_rs232.pointer.ptr_who=who;
    pthread_create(&thread_array[2].thread_ID ,NULL,&Send_Recieve_rs232_thread,&data_rs232 );    ///  start watku do komunikacji rs232
    thread_array[2].thread_name="RS232_thread";


    int SERVER_PORT = atoi(server_settings.PORT.c_str());
    server_settings.SERVER_IP = conv_dns(server_settings.SERVER_IP);
    const char *SERVER_IP = server_settings.SERVER_IP.c_str();

    node_data.server_settings=&server_settings;
    //node_data.pointer.ptr_buf=bufor;
    node_data.pointer.ptr_who=who;

    node_data.main_THREAD_arr = &thread_array[0];

     // start watku camera
    pthread_create (&thread_array[0].thread_ID, NULL,&camera_thread ,&node_data);
    thread_array[0].thread_name="CAMERA_master";
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "watek  camera wystartowal   "<< thread_array[0].thread_ID << std::endl;
    log_file_mutex.mutex_unlock();
    pthread_detach( thread_array[0].thread_ID );
    bzero( & server, sizeof( server ) );

    server.sin_family = AF_INET;
    server.sin_port = htons( SERVER_PORT );
    if( inet_pton( AF_INET, SERVER_IP, & server.sin_addr ) <= 0 )
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

        ////////////////////////   jest połacznie   wiec wstawiamy je  do nowego watku  i  umieszczamy id watku w tablicy  w pierwszym wolnym miejscy ////////////////////
        for (int con_counter=0; con_counter< MAX_CONNECTION; ++con_counter)
        {
            if ( thread_array[con_counter].thread_ID==0 || pthread_kill(thread_array[con_counter].thread_ID, 0) == ESRCH )   // jesli pozycja jest wolna (0)  to wstaw tam  jesli jest zjęta wyslij sygnal i sprawdz czy waŧek żyje ///

            {
                if ( con_counter!=MAX_CONNECTION -1)
                {

                    node_data.s_client_sock =v_sock_ind;
                    node_data.from=from;

                    pthread_create (&thread_array[con_counter].thread_ID, NULL,&Server_connectivity_thread,&node_data);
                    thread_array[con_counter].thread_name = inet_ntoa(node_data.from.sin_addr);
                    thread_array[con_counter].thread_socket=v_sock_ind;
                    log_file_mutex.mutex_lock();
                    log_file_cout << INFO << " watek wystartowal  "<< thread_array[con_counter].thread_ID << std::endl;
                    log_file_mutex.mutex_unlock();

                    pthread_detach( thread_array[con_counter].thread_ID );
                    break;

                }
                else
                {

                    log_file_mutex.mutex_lock();
                    log_file_cout << INFO << " za duzo klientow "<< thread_array[con_counter].thread_ID << std::endl;
                    log_file_mutex.mutex_unlock();



                    if(( send( v_sock_ind, "za duzo kientow \nEND.\n",22 , MSG_DONTWAIT ) ) <= 0 )
                    {
                        perror( "send() ERROR" );
                        break;
                    }

                    continue;

                }

            }

        }
    } // while
    // zamykam gniazdo

    log_file_mutex.mutex_lock();
    log_file_cout << INFO << "zamykanie gniazda wartosc ind "  << shutdown( v_sock_ind, SHUT_RDWR )<< std::endl;
    log_file_cout << ERROR << "gniazdo ind  "<<strerror(  errno ) << std::endl;
    log_file_cout << INFO << "zamykanie gniazda wartosc "  << shutdown( v_socket, SHUT_RDWR )<< std::endl;
    log_file_cout << ERROR << "gniazdo ind  "<<strerror(  errno ) << std::endl;
    log_file_cout << INFO << " koniec programu  "<<   std::endl;
    log_file_mutex.mutex_unlock();

    sleep(3);


    //pthread_join(main_th ,NULL);
    pthread_mutex_destroy(&Logger::mutex_log);
    pthread_mutex_destroy(&C_connection::mutex_buf);
    pthread_mutex_destroy(&C_connection::mutex_who);
    pthread_mutex_destroy(&blockQueue::mutex_queue_char);

    return 0;
}

