#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
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
void dostuff(int); /* function prototype */
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	
	 
	 
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd,
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0)
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     close(sockfd);
     return 0; /* we never get here */
}

void analyse ( char c_buffer [256],SerialPi * serial_ardu)
{

	std::string buffer = "";	
   volatile int value =0;
    printf("\nwiaodmosc: %s\n",c_buffer);
    std::string buf(c_buffer);
    std::vector <std::string> command;

    boost::char_separator<char> sep("=:;");
    boost::tokenizer< boost::char_separator<char> > tokens(buf, sep);
    BOOST_FOREACH (const std::string& t, tokens) {
        //std::cout << " rozmiar t: " << t.size() << std::endl;
        command.push_back( t);
    }
    command.pop_back();  // usowa ostanit wpis smiec

    for (int i=3; i < 7;++i){
        std::cout <<"komenda "<<i<< " to "<< command[i]<<std::endl;
    }
 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 char input = 'o';
		
		if (atoi(command[4].c_str())<0 && command[5] == "0" )
		{
			buffer = "forward:";
			buffer += intToStr( atoi(command[4].c_str())*-1);
			buffer +=";";
		}
		 
		else if (atoi(command[3].c_str()) > 0 && command[4] == "0")
		{
			buffer = "right:";
			buffer += command[3]  ;
			buffer +=";";
			 
		}
		else if (atoi(command[3].c_str()) < 0&& command[4] == "0")
		{
			buffer = "left:";
			buffer +=   intToStr( atoi(command[3].c_str())*-1);
			buffer +=";";
			 
		}
		else if ( command[4]  == "0")
		{
			buffer = "STOP:00000;";
			 
		}
		
		else if (atoi(command[4].c_str()) > 0&& command[5] == "0")
		{
			 
			buffer = "back:";
			buffer +=  command[4]  ;
			buffer +=";";
			 
		}
		else if (atoi(command[5].c_str())>0 && command[4] != "0")
		{
			buffer = "t_right:";
			buffer +=  command[5]  ;;
			buffer +=";";
		}
		else if (atoi(command[5].c_str())<0 && command[4] != "0")
		{
			buffer = "t_left:";
			buffer += intToStr( atoi(command[5].c_str())*-1);
			buffer +=";";
		}
		
		serial_ardu->print(buffer.c_str());
	std::cout << "bufor" << buffer <<std::endl;
		 if (serial_ardu->available() > 0)
            {
               buffer.erase();

               std::cout << " jest bffer : " << buffer << std::endl;
			   while (serial_ardu->available() < 1){
				   std::cout << "wait ";
			   }
			   
                while (serial_ardu->available() > 1){

                    buffer+=serial_ardu->read();

                }
            }
            std::cout << " odebralem: " << buffer << std::endl;
    

}

/******** DOSTUFF() *********************
 There is a separate instance of this function
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock)
{
   int n;
   char buffer[256];
   
	std::string adres ="/dev/ttyACM0";
	
	 SerialPi serial_ardu(strdup(adres.c_str()));
	 serial_ardu.begin(9600);
   while (1){
       bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");


   analyse(buffer,& serial_ardu);



  // sleep(2);


   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
   }
}
