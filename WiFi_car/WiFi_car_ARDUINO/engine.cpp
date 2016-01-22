#include "engine.hpp"
Engine::Engine()
{
}
Engine::~Engine()
{
}
void Engine::init(int pwmA, int pwmB,int BN1,int BN2,int AN1, int AN2)
{
 this -> pwmA=pwmA;
 this -> pwmB=pwmB;
 this -> BN1=BN1;
 this -> BN2=BN2;
 this -> AN1=AN1;
 this -> AN2=AN2;
 
  pinMode(pwmA,OUTPUT);
  pinMode(pwmB,OUTPUT);
  pinMode(BN1,OUTPUT);
  pinMode(BN2,OUTPUT);
  pinMode(AN1,OUTPUT);
  pinMode(AN2,OUTPUT);
  hard_stop();

}

 void Engine::turn_left (int deflexion){
  analogWrite (pwmA, power);
  analogWrite (pwmB, power-deflexion);
  run_time=millis();
 }
 void Engine::turn_right (int deflexion){
  analogWrite (pwmB, power);
  analogWrite (pwmA, power-deflexion);
  run_time=millis();
 }
void Engine::set_power (int power){
  if (power > 255)
  {
    power = 255;
  }
  else{
  this -> power = power;
  }
 analogWrite (pwmA, power);
 analogWrite (pwmB, power);
run_time=millis();
}
 void Engine::timeout (long _time){
  if (millis()-run_time > _time){
    hard_stop();
  }
 }
 void Engine::set_run_time (unsigned long int t){
  run_time=t;
 }
 void  Engine::rotation_right(){
  set_power(0);
  
  digitalWrite(AN2,LOW);
  digitalWrite(AN1,HIGH);
  
  digitalWrite(BN2,LOW);
  digitalWrite(BN1,HIGH);
  
  run_time=millis();
 }
 void  Engine::rotation_left(){
  set_power(0);
  digitalWrite(AN1,LOW);
  digitalWrite(AN2,HIGH);
  digitalWrite(BN1,LOW);
  digitalWrite(BN2,HIGH);
  run_time=millis();
 }
  void Engine::go_forward(){
    set_power(0);
 
  digitalWrite(AN1,LOW);
  digitalWrite(AN2,HIGH);
  
  digitalWrite(BN2,LOW);
  digitalWrite(BN1,HIGH);
  run_time=millis();  
  }
 void Engine::go_back(){
  set_power(0);
  
  
  digitalWrite(AN2,LOW);
  digitalWrite(AN1,HIGH);
  digitalWrite(BN1,LOW);
  digitalWrite(BN2,HIGH);
   
 run_time=millis();
 }
 void Engine::hard_stop (){
  set_power(0);
  analogWrite (pwmB,power);
  analogWrite (pwmA,power);
  digitalWrite(AN2,LOW);
  digitalWrite(AN1,LOW);
  digitalWrite(BN2,LOW);
  digitalWrite(BN1,LOW);
 }

