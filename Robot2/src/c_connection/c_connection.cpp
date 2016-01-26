#include "c_connection.h"
#include <iostream>
//using namespace std;



pthread_mutex_t C_connection::mutex_buf = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t C_connection::mutex_who = PTHREAD_MUTEX_INITIALIZER;

// konstruktor
C_connection::C_connection (thread_data  *my_data):c_socket(my_data->s_client_sock),
    c_from(my_data->from)
{

    this -> pointer = &my_data->pointer;
    this -> my_data = my_data;
    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< " konstruuje nowy obiekt do komunikacj na gniezdzie " << c_socket <<  std::endl;
    log_file_mutex.mutex_unlock();

}



// destruktor
C_connection::~C_connection()
{
    shutdown( c_socket, SHUT_RDWR );
    for (int i =0 ; i< MAX_CONNECTION;++i)
    {
        if (my_data->main_THREAD_arr[i].thread_ID == pthread_self())
        {
            my_data->main_THREAD_arr[i].thread_ID = 0;
            my_data->main_THREAD_arr[i].thread_name ="  -empty-  ";
            my_data->main_THREAD_arr[i].thread_socket=0;
            break;
        }
    }
    log_file_mutex.mutex_lock();
    log_file_cout << INFO<< " koniec komunikacji - kasuje obiekt" <<  std::endl;
    log_file_mutex.mutex_unlock();
}

int C_connection::c_send(int para)
{
    std::cout << " wielkosc bufora "<< strlen(c_buffer) << std::endl;
    if(( send( c_socket, c_buffer ,strlen(c_buffer), para ) ) <= 0 )
    {
        //perror( "send() ERROR" );
        return -1;
    }

    return 0;
}

int C_connection::c_send(char command[MAX_buf])
{
    c_write_buf(command);
    c_send(0);
    return 0;
}

int C_connection::c_recv(int para)
{
    for (unsigned int i =0 ; i< sizeof(c_buffer);++i)
    {
        c_buffer[i]=' ';
    }
    recv_size = recv( c_socket, c_buffer , MAX_buf, para );
    if(recv_size < 0 )
    {
        //perror( "recv() ERROR" );
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << " recv() error" <<   std::endl;
        log_file_mutex.mutex_unlock();
        return -1;
    }
    else if (recv_size == 0)
    {
        log_file_mutex.mutex_lock();
        log_file_cout << INFO << "gniazdo zamkniete" <<   std::endl;
        log_file_mutex.mutex_unlock();
        return -1;
    }

    std::cout << "giazdo odebralo bajtow: " <<recv_size << std::endl;
    for  ( int i =0 ; i< recv_size;++i)
    {
        std::cout << c_buffer[i];
    }
    std::cout << " " <<std::endl;
    return 1;
}






int C_connection::c_analyse()
{
    std::string buf(c_buffer);
    std::vector <std::string> command;

    boost::char_separator<char> sep(",\n ");
    boost::tokenizer< boost::char_separator<char> > tokens(buf, sep);

    BOOST_FOREACH (const std::string& t, tokens) {
        std::cout << " rozmiar t: " << t.size() << std::endl;
        command.push_back( t);
    }
    command.pop_back();  // usowa ostanit wpis smiec


    switch (command.size())
    {
    case 1 :
        if(command[0]=="exit")
        {
            std::cout << " klient sie odlaczyl" << std::endl;
            c_write_buf("\nEND.\n");
            break;
        }
        else if (command[0]=="hello")
        {
            c_write_buf("\nHI !\n");
            break;
        }
        else if (command [0] == "help")
        {
            l_send_log("/etc/config/iDom_SERVER/help");
            //break;
        }
        else if (command [0] == "OK")
        {
            c_write_buf("\nEND.\n");
            break;
        }
        else if (command [0] == "IP")
        {
            c_write_buf(( char*) my_data->server_settings->SERVER_IP.c_str() );
            break;
        }
        else if (command [0] == "uptime")
        {
            time(&my_data->now_time);
            temporary_str ="uptime: ";
            temporary_str += boost::lexical_cast<std::string>( difftime(my_data->now_time,my_data->start)    ) + " sek.";

            c_write_buf( (char*) temporary_str.c_str() );
            break;
        }
        else
        {
            c_write_buf("\nEND.\n");
            break;
        }
    case 2 :
        if(command[0]=="stop")
        {
            std::cout << " jest stop";
            if (command[1]=="server")
            {
                std::cout <<"wychodze!!!!";
                c_send("\nEND.\n");

                return false;

            }
            break;
        }
        else if (command[0]=="show")
        {
            if (command[1]=="log")
            {

                l_send_log(_logfile);
                c_write_buf("\nEND.");
                break;
            }

        }

        else
        {
            c_write_buf("\nEND.\n");
            break;
        }


    case 3:

        if (command[0]=="RS232")
        {
            if (command[1]=="get")
            {
                if (command[2]=="temperature"){
                    std::cout << " szukam temeratury" << std::endl;

                            c_write_buf(  (char *)send_to_arduino(my_data,"temperature:339;").c_str()    );

                    break;
                }
                else
                {
                    c_write_buf((char*)("wrong parameter: "+command[2]).c_str());
                    break;
                }


            }

            else if (command[1]=="send")
            {
                c_write_buf(  (char *)send_to_arduino(my_data,command[2]).c_str()    );
                break;
            }
            else
            {
                c_write_buf((char*)("wrong parameter: "+command[1]).c_str());
                break;
            }

        }

        else if (command[0]=="show")
        {
            if (command[1]=="thread")
            {
                if (command [2] !="all"){
                    temporary_str = my_data->main_THREAD_arr[atoi(command[2].c_str())].thread_name;
                    temporary_str += " ID: ";
                    temporary_str += intToStr(my_data->main_THREAD_arr[atoi(command[2].c_str())].thread_ID);
                    temporary_str += " socket: ";
                    temporary_str += intToStr(my_data->main_THREAD_arr[atoi(command[2].c_str())].thread_socket);
                    c_write_buf( (char*)temporary_str.c_str() );
                break;
                }

                else {
                    for (int i =0 ; i< MAX_CONNECTION;++i)
                    {
                        temporary_str = intToStr(i)+"\t";
                        temporary_str += my_data->main_THREAD_arr[i].thread_name;
                        temporary_str += "\t ID: ";
                        temporary_str += intToStr(my_data->main_THREAD_arr[i].thread_ID);

                        if (my_data->main_THREAD_arr[i].thread_socket !=0){
                            temporary_str += " socket: ";
                            temporary_str += intToStr(my_data->main_THREAD_arr[i].thread_socket);
                        }

                        temporary_str += "\n";
                        c_write_buf( (char*)temporary_str.c_str() );
                        c_send(0);
                        c_recv(0);

                    }
                    c_write_buf("\nEND.");

                    break;
                }
            }

        }

        else
        {
            c_write_buf("END.\n");
            break;
        }

    default :
        std::cout << " nic nie przyszlo komenda z dupy " << c_buffer<<std::endl;
        c_write_buf("unknown command\n");

    }


    return true;
}

void C_connection::c_send_recv_MASTER()
{


}



void C_connection::c_send_recv_RS232()
{

}


