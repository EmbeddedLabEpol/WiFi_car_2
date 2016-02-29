#include <Arduino.h>
class Engine {
 public:
  Engine();
  ~Engine();
 void init(int pwmA, int pwmB,int BN1,int BN2,int AN1, int AN2);
 void turn_left (int deflexion);
 void turn_right (int deflexion);
 void set_power (int power);
 void timeout (long _time);
 void rotation_right();
 void rotation_left();
 void go_forward();
 void go_back();
 void hard_stop();
 void set_run_time (unsigned long int t);
 private:
 volatile unsigned int power;
 volatile unsigned long run_time;
 int pwmA;
 int pwmB;
 int BN1;
 int BN2;
 int AN1;
 int AN2;
  
};
