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
    log_file_cout << INFO<< "konstruuje nowy obiekt do komunikacj na gniezdzie " << c_socket <<  std::endl;
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
    log_file_cout << INFO<< "koniec komunikacji - kasuje obiekt" <<  std::endl;
    log_file_mutex.mutex_unlock();
}

int C_connection::c_send(int para)
{
    if(( send( c_socket, str_buf.c_str() ,str_buf.length(), para ) ) <= 0 )
    {
        return -1;
    }

    return 0;
}

int C_connection::c_send(std::string command )
{
    str_buf = command;
    c_send(0);
    return 0;
}

int C_connection::c_recv(int para)
{
    for (unsigned int i =0 ; i< sizeof(c_buffer);++i)
    {
        c_buffer[i]=' ';
    }

    struct timeval tv;
    tv.tv_sec = 90;
    tv.tv_usec = 0;
    setsockopt(c_socket,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv , sizeof(struct timeval));

    recv_size = recv( c_socket, c_buffer , MAX_buf, para );

    if(recv_size < 0 )
    {
        //perror( "recv() ERROR" );
        log_file_mutex.mutex_lock();
        log_file_cout << ERROR << "recv() error - " << strerror(  errno ) <<   std::endl;
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

   /* std::cout << "giazdo odebralo bajtow: " <<recv_size << std::endl;
    for  ( int i =0 ; i< recv_size;++i)
    {
        std::cout << c_buffer[i];
    }
    std::cout << " " <<std::endl;*/
    return 1;
}






int C_connection::c_analyse()
{
    std::string buf(c_buffer);
    std::vector <std::string> command;

    boost::char_separator<char> sep(",:; ");
    boost::tokenizer< boost::char_separator<char> > tokens(buf, sep);

    BOOST_FOREACH (const std::string& t, tokens) {
        //std::cout << " rozmiar t: " << t.size() << std::endl;
        command.push_back( t);
    }
    command.pop_back();  // usowa ostanit wpis smiec
     str_buf= "unknown command\n";

    switch (command.size())
    {
    case 1 :
        if(command[0]=="exit")
        {
            str_buf="\nEND.\n";
            break;
        }

        else if (command[0]=="hello")
        {
            str_buf = "\nHI !\n";
            break;
        }
        else if (command [0] == "help")
        {
            l_send_file("/etc/config/iDom_SERVER/help");
            str_buf="\nEND.\n";
            break;
        }
        else if (command [0] == "OK")
        {
            str_buf = "\nEND.\n";
            break;
        }
        else if (command [0] == "IP")
        {
            str_buf = my_data->server_settings->SERVER_IP;

            break;
        }
        else if (command [0] == "uptime")
        {
            time(&my_data->now_time);
            str_buf ="uptime: ";
            str_buf +=  sek_to_uptime(difftime(my_data->now_time,my_data->start) );
            break;
        }
        else
        {
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
            else
            {   str_buf="stop what? \n";
                break;
            }
        }
        else if (command[0]=="show")
        {
            if (command[1]=="log")
            {
                l_send_file(_logfile);
                str_buf="\nEND.\n";
                break;
            }
            else if (command[1]=="thread")
            {
                str_buf = " No ID";
                break;
            }

        }

        else
        {
            // c_write_buf("\nEND.\n");
            break;
        }


    case 3:

        if (command[0]=="RS232")
        {
            if (command[1]=="get")
            {
                if (command[2]=="temperature"){
                    std::cout << " szukam temeratury" << std::endl;

                    str_buf = send_to_arduino(my_data,"temperature:339;");
                    break;
                }
                else
                {
                    str_buf = ("wrong parameter: "+command[2]);
                    break;
                }


            }

            else if (command[1]=="send")
            {
                str_buf = send_to_arduino(my_data,command[2]);
                break;
            }
            else
            {
                str_buf = ("wrong parameter: "+command[1]);
                break;
            }

        }

        else if (command[0]=="show")
        {
            if (command[1]=="thread")
            {
                if (command [2] !="all"){
                    str_buf  = my_data->main_THREAD_arr[atoi(command[2].c_str())].thread_name;
                    str_buf  += " ID: ";
                    str_buf += intToStr(my_data->main_THREAD_arr[atoi(command[2].c_str())].thread_ID);
                    str_buf  += " socket: ";
                    str_buf  += intToStr(my_data->main_THREAD_arr[atoi(command[2].c_str())].thread_socket);

                    break;
                }

                else {
                    for (int i =0 ; i< MAX_CONNECTION;++i)
                    {
                        str_buf  = intToStr(i)+"\t";
                        str_buf  += my_data->main_THREAD_arr[i].thread_name;
                        str_buf  += "\t ID: ";
                        str_buf  += intToStr(my_data->main_THREAD_arr[i].thread_ID);

                        if (my_data->main_THREAD_arr[i].thread_socket !=0){
                            str_buf  += " socket: ";
                            str_buf  += intToStr(my_data->main_THREAD_arr[i].thread_socket);
                        }

                        str_buf  += "\n";

                        c_send(0);
                        c_recv(0);

                    }
                    str_buf ="\nEND.";

                    break;
                }
            }

        }

        else
        {
            // c_write_buf("END.\n");
            break;
        }


    
    case 24:
        buffer = "STOP:00000;";
        if (atoi(command[4].c_str())<0 && atoi(command[5].c_str()) ==0 && command[13]=="0" )
        {
            buffer = "forward:";
            buffer += intToStr( atoi(command[4].c_str())*-1);
            buffer +=";";
        }

        else if (atoi(command[3].c_str()) > 0 &&  atoi(command[4].c_str()) ==0  && command[13]=="0")
        {
            buffer = "right:";
            buffer += command[3]  ;
            buffer +=";";

        }
        else if (atoi(command[3].c_str()) < 0&&  atoi(command[4].c_str()) ==0  && command[13]=="0")
        {
            buffer = "left:";
            buffer +=   intToStr( atoi(command[3].c_str())*-1);
            buffer +=";";

        }
       
        else if (atoi(command[4].c_str()) > 0&&  atoi(command[5].c_str()) ==0  && command[13]=="0")
        {

            buffer = "back:";
            buffer +=  command[4]  ;
            buffer +=";";

        }
        else if (atoi(command[5].c_str())>0 &&  atoi(command[4].c_str()) !=0 && command[13]=="0")
        {
            buffer = "t_left:";
            buffer +=  command[5]  ;
            buffer +=";";
        }
        else if (atoi(command[5].c_str())<0 &&  atoi(command[4].c_str()) !=0  && command[13]=="0")
        {
            buffer = "t_right:";
            buffer += intToStr( atoi(command[5].c_str())*-1);
            buffer +=";";
        }
        //////////////////////// ruszanie kamera

        else if (  command[11]=="1")
        {
            buffer = "h_move_left:";
            buffer += "3"  ;
            buffer +=";";

        }
        else if (   command[9]=="1")
        {
            buffer = "h_move_right:";
            buffer +=  "3"  ;
            buffer +=";";
        }





        else if (  command[8]=="1")
        {
            buffer = "v_move_left:";
            buffer += "3"  ;
            buffer +=";";

        }
        else if (   command[10]=="1")
        {
            buffer = "v_move_right:";
            buffer +=  "3"  ;
            buffer +=";";
        }
        else if (   command[13]=="1" && command[15]=="1")
        {
            buffer = "_move_zero:";
            buffer +=  "3"  ;
            buffer +=";";
        }
        str_buf = send_to_arduino(my_data, buffer ) ;

        break;
    default :
        std::cout << " nic nie przyszlo komenda z dupy " << c_buffer<<std::endl;
        str_buf ="unknown command\n" ;

    }


    return true;
}

void C_connection::c_send_recv_MASTER()
{


}



void C_connection::c_send_recv_RS232()
{

}


