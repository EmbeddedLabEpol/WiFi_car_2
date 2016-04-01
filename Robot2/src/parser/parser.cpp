#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "parser.hpp"


config read_config  ( const char* file_path    )
{
    std::string s_mac="MAC1";
    std::string s_node="NODE1";
    std::string content ;
    std::string variable ;
    std::string v_value ;
    std::fstream config_file;

    config  v_set;

    config_file.open(file_path, std::ios::in  );
    if( config_file.good() == false )
    {
        log_file_mutex.mutex_lock();
        log_file_cout << "Brak pliku konfiguracyjnego" << std::endl;
        log_file_mutex.mutex_unlock();

        return v_set;
    }

    while( !config_file.eof() )
    {
        getline (config_file, content);

        for (unsigned i=0;  i<content.length(); ++i)
        {
            if ( content.at(i)==':' && content.at(i+1)=='=')
            {   for (unsigned int j=i+2;j<content.length(); ++j )
                {
                    if ( content.at(j)==' '|| content.at(j)==0x9 ){   // przerwij odczyt  jesli znajdzesz spacje lub tab
                        break;
                    }
                    // std::cout << (int)content.at(j);
                    v_value.push_back(content.at(j));  // v_value zmiennej
                }

                // std::cout << std::endl;
                break;
            }
            if (content.at(i)!=' ' )
            {
                variable.push_back(content.at(i));  // variable
            }
        }

        if (variable == "portRS232")
        {
            v_set.portRS232= v_value;
        }
        if (variable == "ID")
        {


            v_set.ID_server = atoi(  v_value.c_str());
        }
        if (variable == "SERVER_IP")
        {
            v_set.SERVER_IP= v_value;
        }


        if (variable == "BaudRate")
        {
            v_set.BaudRate= v_value;
        }
        if (variable == "DELAY")
        {
            v_set.v_delay  = atoi(v_value.c_str());
        }
        if (variable == "PORT")
        {
            v_set.PORT= v_value;
        }

    variable = v_value = "";
    }




    config_file.close();



    return v_set;
}
