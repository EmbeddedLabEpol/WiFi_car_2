
#include "engine.hpp"
#define LED_RED 3
#define LED_GREEN 2

volatile unsigned int speed_car = 0;
Engine motor;
unsigned long led_time = 0;
String command;
String value;
;

void setup() {
  Serial.begin(9600);

  Serial.print ("dziala");
  motor.init(10, 9, 8, 7, 6, 5);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void led_blink() {
  if (millis() - led_time < 500 ) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    //Serial.println("GREEN");
  }
  else if (millis() - led_time < 1000 && millis() - led_time > 499)
  {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    // Serial.println("RED");
  }
  else
  {
    led_time = millis();
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  if (Serial.available() > 6) {


   // Serial.println("START");
    command =  Serial.readStringUntil(':');

    //Serial.println(command);

    value =  Serial.readStringUntil(';');
    speed_car = value.toInt();
    delay(10);
    while (Serial.available()) {
      Serial.read();
    }

    if (command == "forward" )
    {

      
      motor.go_forward();
      motor.set_power(speed_car);
      Serial.println(speed_car);
      Serial.println("OK");
    }

    else if (command == "fast" )
    {
      
      if (speed_car < 245) {
        speed_car += 10;
      }
      Serial.println("OK");
      motor.set_run_time(millis());
    }

    else if (command == "slow")
    {
       
      if ( speed_car > 20) {
        speed_car -= 10;
      }
      Serial.println("OK");
      motor.set_run_time(millis());
    }


    else if (command == "t_left")
    {
      Serial.println("OK");
      motor.turn_left(  speed_car);

    }

    else if (command == "t_right")
    {
      Serial.println("OK");
      motor.turn_right(speed_car);
    }




    else if (command == "back")
    {
       
      motor.go_back();
      motor.set_power(speed_car);
      Serial.println("OK");
       
    }
    else if (command == "STOP")
    {
       
      motor.hard_stop();
      Serial.println("OK");
    }
    else if (command == "left" )
    {
       
      motor.rotation_left();
      motor.set_power(speed_car);
      Serial.println("OK");
    }
    else if ( command == "right")
    {
       
      motor.rotation_right();
      motor.set_power(speed_car);
      Serial.println("OK");
    }

  }
  led_blink();
  motor.timeout(500);
}

