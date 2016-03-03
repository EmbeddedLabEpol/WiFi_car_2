#include <Servo.h>
#include "servo_camera.h"
#include "HC_SR04.h"
#include "engine.h"
#define LED_RED 12
#define LED_GREEN 10 
 
int Trig = A1;  //pin 2 Arduino połączony z pinem Trigger czujnika
int Echo = A0;  //pin 3 Arduino połączony z pinem Echo czujnika
#define ECHO_INT 0
HC_SR04 sensor(Trig,Echo,ECHO_INT); 
int cm_dis=30;        //odległość w cm
long t_time;     //długość powrotnego impulsu w uS
int dis = 20;
volatile unsigned int speed_car = 0;

Engine motor;
servo_cam my_servo;
unsigned long led_time = 0;

unsigned long  dis_time = 0;
String command;
String value;


void setup() {
  Serial.begin(9600);
  sensor.begin();
  //Serial.print ("dziala");
  motor.init(3, 5, 8, 7, 6, 4);
  my_servo.init (13,11 );

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(Trig, OUTPUT);                     //ustawienie pinu 2 w Arduino jako wyjście
  pinMode(Echo, INPUT);                      //ustawienie pinu 3 w Arduino jako wejście
   sensor.start();
}
void pomiar_odleglosci ()
{
  
  if (millis() - led_time < 1000) {
  /*    digitalWrite(Trig, LOW);       
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);       //ustawienie stanu wysokiego na 10 uS - impuls inicjalizujacy - patrz dokumentacja
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  t_time = pulseIn(Echo, HIGH);
  cm_dis = t_time / 58; */
  if (sensor.isFinished()) {
  cm_dis = sensor.getRange();
  delay(50);
  sensor.start();
  }  
  }
   
  else
  {
    led_time = millis();
  }
               //szerokość odbitego impulsu w uS podzielone przez 58 to odleglosc w cm - patrz dokumentacja
}  

void led_blink() {
  if (millis() - led_time < 100 ) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    //Serial.println("GREEN");
  }
  else if (millis() - led_time < 200 )
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

    if (command == "forward" && cm_dis> dis )
    {
     
      motor.go_forward();
      motor.set_power(speed_car);
      my_servo._move_zero(0);
      Serial.println("FORWAR");
      Serial.print(cm_dis);
      Serial.println(";");
    }

    else if (command == "t_left" && cm_dis > dis)
    {
      Serial.println("t_left");
      Serial.print(cm_dis);
      Serial.println(";");
      motor.turn_left(  speed_car);
      my_servo.h_move_left_turn();

    }

    else if (command == "t_right"  && cm_dis > dis)
    {
      Serial.println("t_right");
      Serial.print(cm_dis);
      Serial.println(";");
      motor.turn_right(speed_car);
      my_servo.h_move_right_turn();
    }




    else if (command == "back")
    {
       
      motor.go_back();
      motor.set_power(speed_car);
      Serial.println("back");
      Serial.print(cm_dis);
      Serial.println(";");
      my_servo._move_zero(1);
       
    }
    else if (command == "STOP")
    {
       
      motor.hard_stop();
      Serial.println("STOP");
      Serial.print(cm_dis);
      Serial.println(";");
    }
    else if (command == "left" )
    {
       
      motor.rotation_left();
      motor.set_power(speed_car);
      Serial.println("LEFT");
      Serial.print(cm_dis);
      Serial.println(";");
    }
    else if ( command == "right")
    {
       
      motor.rotation_right();
      motor.set_power(speed_car);
      Serial.println("RIGHT");
      Serial.print(cm_dis);
      Serial.println(";");
    }
    else if (command == "v_move_left")
    {
      Serial.println("servo_V");
      Serial.print(cm_dis);
      Serial.println(";");
      my_servo.v_move_left(speed_car);
      
    }
    else if (command == "h_move_left")
    {
      Serial.println("servo_H");
      Serial.print(cm_dis);
      Serial.println(";");
      my_servo.h_move_left(speed_car);
    }
 else if (command == "v_move_right")
    {
      Serial.println("servo_V");
      Serial.print(cm_dis);
      Serial.println(";");
      my_servo.v_move_right(speed_car);
    }
    else if (command == "h_move_right")
    {
      Serial.println("servo_H");
      Serial.print(cm_dis);
      Serial.println(";");
      my_servo.h_move_right(speed_car);
    }
     else if (command == "_move_zero")
    {
      Serial.println("zero");
      Serial.print(cm_dis);
      Serial.println(";");
      my_servo._move_zero(0);
    }
    else
    {
      Serial.print("unknown_command");
      Serial.print(command);
      Serial.print(cm_dis);
      Serial.println(";");
    }

  }
  pomiar_odleglosci();              //pomiar odległości
  
  //Serial.print(CM);
  //Serial.print(" cm");
  //Serial.println(";");
  led_blink();
  motor.timeout(500);
}

