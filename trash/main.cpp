#include <iostream> 
#include <stdio.h> 
#include <cstdlib> 
#include <stdio.h>
#include "arduPi/arduPi.h"
std::string intToStr(int n)
{
     std::string tmp, ret;
     if(n < 0) {
      ret = "-";
      n = -n;
     }
     do {
      tmp += n % 10 + 48;
      n -= n % 10;
     }
     while(n /= 10);
     for(int i = tmp.size()-1; i >= 0; i--)
      ret += tmp[i];
     return ret;
}
int main() { 
    system("stty raw"); // Set terminal to raw mode
    std::string buffer = "";	
	std::string adres ="/dev/ttyACM0";
	volatile int value =0;
	 SerialPi serial_ardu(strdup(adres.c_str()));
	 serial_ardu.begin(9600);
    while(1)
    {
		char input = getchar(); 
		system("clear");

		std::cout << std::endl << "Got message: " << input << std::endl;
		if (input == 'p')
			break;
		
		if (input == 'w')
		{
			buffer = "forward:";
			buffer += intToStr(value);
			buffer +=";";
		}
		else if (input == '8')
		{
			value = 255;
		}
		else if (input == '2')
		{
			value = 150;
		}
		else if (input == '4')
		{
			value = 180;
		}
		else if (input == 'a')
		{
			buffer = "left:";
			buffer += intToStr(value);
			buffer +=";";
		}
		else if (input == 's')
		{
			buffer = "STOP:000;";
		}
		else if (input == 'd')
		{
			buffer = "right:";
			buffer += intToStr(value);
			buffer +=";";
		}
		else if (input == 'z')
		{
			buffer = "back:";
			buffer += intToStr(value);
			buffer +=";";
		}
		else if (input == 'q')
		{
			buffer = "t_right:";
			buffer += intToStr(value);
			buffer +=";";
		}
		else if (input == 'e')
		{
			buffer = "t_left:";
			buffer += intToStr(value);
			buffer +=";";
		}
		
		serial_ardu.print(buffer.c_str());
		
		 if (serial_ardu.available() > 0)
            {
               buffer.erase();

               std::cout << " jest bffer : " << buffer << std::endl;
                while (serial_ardu.available() > 0){

                    buffer+=serial_ardu.read();

                }
            }
            std::cout << " odebralem: " << buffer << std::endl;
    }
    system("stty cooked"); // Reset terminal to normal "cooked" mode 
    return 0; 
}