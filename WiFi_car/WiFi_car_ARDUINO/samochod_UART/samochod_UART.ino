#include <Servo.h>
#include "servo_camera.h"

#include "engine.h"


#define LED_RED 12
#define LED_GREEN 10

volatile unsigned int speed_car = 0;

Engine motor;
servo_cam my_servo;
unsigned long led_time = 0;
String command;
String value;
;

void setup() {
  Serial.begin(9600);

  Serial.print ("dziala");
  motor.init(3, 5, 8, 7, 6, 4);
  my_servo.init (13,11 );

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void led_blink() {
  if (millis() - led_time < 50 ) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    //Serial.println("GREEN");
  }
  else if (millis() - led_time < 100 )
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


    
    command =  Serial.readStringUntil(':');

    
    

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
      my_servo._move_zero(1);
      Serial.println("FORWAR");
      Serial.println(";");
    }



    else if (command == "t_left")
    {
      Serial.println("t_left");
      Serial.println(";");
      motor.turn_left(  speed_car);
      my_servo.h_move_left_turn();

    }

    else if (command == "t_right")
    {
      Serial.println("t_right");
      Serial.println(";");
      motor.turn_right(speed_car);
      my_servo.h_move_right_turn();
    }




    else if (command == "back")
    {
       
      motor.go_back();
      motor.set_power(speed_car);
      Serial.println("back");
      Serial.println(";");
      my_servo._move_zero(1);
       
    }
    else if (command == "STOP")
    {
       
      motor.hard_stop();
      Serial.println("STOP");
      Serial.println(";");
    }
    else if (command == "left" )
    {
       
      motor.rotation_left();
      motor.set_power(speed_car);
      Serial.println("LEFT");
      Serial.println(";");
    }
    else if ( command == "right")
    {
       
      motor.rotation_right();
      motor.set_power(speed_car);
      Serial.println("RIGHT");
      Serial.println(";");
    }
    else if (command == "v_move_left")
    {
      Serial.println("servo_V");
      Serial.println(";");
      my_servo.v_move_left(speed_car);
      
    }
    else if (command == "h_move_left")
    {
      Serial.println("servo_H");
      Serial.println(";");
      my_servo.h_move_left(speed_car);
    }
 else if (command == "v_move_right")
    {
      Serial.println("servo_V");
      Serial.println(";");
      my_servo.v_move_right(speed_car);
    }
    else if (command == "h_move_right")
    {
      Serial.println("servo_H");
      Serial.println(";");
      my_servo.h_move_right(speed_car);
    }
     else if (command == "_move_zero")
    {
      Serial.println("zero");
      Serial.println(";");
      my_servo._move_zero(0);
    }
    else
    {
      Serial.print("unknown_command");
      Serial.print(command);
      Serial.println(";");
    }

  }
  led_blink();
  motor.timeout(500);
}

