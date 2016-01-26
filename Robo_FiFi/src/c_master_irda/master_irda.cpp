/*mapa klawiszy pilota i znaki im przypisane
KEY_POWER           P
KEY_0               0
KEY_1               1
KEY_2               2
KEY_3               3
KEY_4               4
KEY_5               5
KEY_6               6
KEY_7               7
KEY_8               8
KEY_9               9
KEY_AUDIO       	A
KEY_CHANNELDOWN     /
KEY_CHANNELUP       ^
KEY_DOWN            D
KEY_EPG             E
KEY_EXIT            e
KEY_FAVORITES       F
KEY_INFO            I
KEY_LANGUAGE        L
KEY_MENU        	M
KEY_MUTE        	m
KEY_OK              O
KEY_RADIO           r
KEY_REFRESH         R
KEY_SAT             s
KEY_SUBTITLE        S
KEY_TEXT            T
KEY_TV              t
KEY_UP              U
KEY_VOLUMEDOWN      -
KEY_VOLUMEUP        +

 */
#include "master_irda.h"

master_irda::master_irda(thread_data *my_data)
{
    buttonTimer = millis();
    buttonMENU = 0;
      my_data2 = my_data;

    //Initiate LIRC. Exit on failure
    if(lirc_init("lirc",1)==-1)
        exit(EXIT_FAILURE);
    std::cout << " jestem po uruchominiu lirc \n";

    //// tu chyba sa glupoty  zle


}
void master_irda::setup ()
{


}

void master_irda::run()
{

    c_irda_logic irda_queue(my_data2);
    //Read the default LIRC config at /etc/lirc/lircd.conf  This is the config for your remote.
    if(lirc_readconfig(NULL,&config,NULL)==0)
    {
        // std::cout << " jestem w iflirc_readconfig(NULL,&config,NULL)==0 \n ";
        //Do stuff while LIRC socket is open  0=open  -1=closed.
        while(lirc_nextcode(&code)==0 )
        {
            std::cout << " w while \n";
            if (go_while==false)     /// TO NIE DZIALA DO POPRAWY
            {
                break;  // koncze petle zamykam wątek
            }
            //If code = NULL, meaning nothing was returned from LIRC socket,
            //then skip lines below and start while loop again.
            if(code==NULL) {
                std::cout << " kontynuuje\n";
                continue;}
            {
                //Make sure there is a 400ms gap before detecting button presses.
                if (millis() - buttonTimer  > 400 ){

                    digitalWrite(BUZZER,ON);

                    // time out   OK menu
                    if (millis() - buttonTimer  > 30000 && buttonMENU == 1){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " timeout menu " <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        buttonMENU=0;  // flaga na 0  nie wejdze juz do timeout
                        irda_queue._add('e');
                    }
                    //////////////////////////////////////////////////////////////
                    //Check to see if the string "KEY_1" appears anywhere within the string 'code'.

                    if(strstr (code,"KEY_POWER")){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " wcisnieto POWER" <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        irda_queue._add('P');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_1")){
                        irda_queue._add('1');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_2")){
                        irda_queue._add('2');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_3")){
                        irda_queue._add('3');
                        //my_data2->mainLCD->printString(1,1,"klawisz 3");
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_4")){
                        irda_queue._add('4');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_5")){
                        irda_queue._add('5');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_6")){
                        irda_queue._add('6');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_7")){
                        irda_queue._add('7');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_8")){
                        irda_queue._add('8');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_9")){
                        irda_queue._add('9');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_0")){
                        irda_queue._add('0');
                        buttonTimer = millis();
                    }
                    ////////////////////////////////////
                    else if(strstr (code,"KEY_INFO")){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " operacja na diodzie" <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        irda_queue._add('I');
                        buttonTimer = millis();
                    }
                    /////////////////////////////////////////////////
                    else if(strstr (code,"KEY_TV")){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " wcisnieto PLAY" <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        irda_queue._add('t');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_AUDIO")){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " wcisnieto PAUSE" <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        irda_queue._add('A');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_VOLUMEUP")){
                        irda_queue._add('+');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_VOLUMEDOWN")){
                        irda_queue._add('-');
                        buttonTimer = millis();
                    }
                    /////////////////////////////////////////////

                    else if(strstr (code,"KEY_DOWN")){
                        irda_queue._add('D');
                        buttonTimer = millis();
                    }

                    else if(strstr (code,"KEY_UP")){
                        irda_queue._add('U');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_OK")){
                        irda_queue._add('O');
                        buttonMENU=0;
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_MENU")){
                        irda_queue._add('M');
                        buttonMENU=1;// wlacz timeout dla menu
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_EXIT")){
                        irda_queue._add('e');
                        buttonMENU = 0;
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_RADIO")){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " sterowanie projektorem" <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        irda_queue._add('r');   //idziemy do sterowania projektorem
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_EPG")){
                        log_file_mutex.mutex_lock();
                        log_file_cout << INFO<< " przegladanie katalogu z filmami" <<  std::endl;
                        log_file_mutex.mutex_unlock();
                        buttonMENU=1;
                        irda_queue._add('E');   //idziemy do sterowania projektorem
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_CHANNELUP")){

                        irda_queue._add('^');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_CHANNELDOWN")){

                        irda_queue._add('/');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_MUTE")){
                        irda_queue._add('m');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_FAVORITES")){
                        irda_queue._add('F');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_REFRESH")){
                        irda_queue._add('R');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_LANGUAGE")){
                        irda_queue._add('L');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_SUBTITLE")){
                        irda_queue._add('S');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_SAT")){
                        irda_queue._add('s');
                        buttonTimer = millis();
                    }
                    else if(strstr (code,"KEY_TEXT")){
                        irda_queue._add('T');
                        buttonTimer = millis();
                    }
                }
            }
            //Need to free up code before the next loop
            free(code);

            digitalWrite(BUZZER,OFF);
        }
        //Frees the data structures associated with config.
        lirc_freeconfig(config);
    }
    log_file_mutex.mutex_lock();
    log_file_cout << INFO << " koniec watku master IRDA  "<<   std::endl;
    log_file_mutex.mutex_unlock();


    //lirc_deinit() closes the connection to lircd and does some internal clean-up stuff.
    lirc_deinit();
    // exit(EXIT_SUCCESS);


}
