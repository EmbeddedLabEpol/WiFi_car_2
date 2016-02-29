
#include "servo_camera.h"

servo_cam::servo_cam()
{
  h_value = 70;
  v_value = 90;
}
servo_cam::~servo_cam()
{
  
}

void servo_cam::init ( int v_axis,int h_axis){
this -> v_axis = v_axis;
this -> h_axis = h_axis;
  v_servo.attach(v_axis);
  h_servo.attach(h_axis);
  
  h_servo.write(h_value);
  v_servo.write(v_value);
  automation = 1;
}// end init 

  void servo_cam::h_move_left(int value){
   if (h_value <140-value)
      {
       h_value+=value; 
      }
      Serial.print(h_value);
   h_servo.write(h_value);
   automation =0;
  }
  
  void servo_cam::v_move_left(int value){
      if (v_value >value)
      {
       v_value-=value; 
      }
      Serial.print(v_value);
    v_servo.write(v_value);
    automation =0;
  }
  
  void servo_cam::h_move_right(int value){
     if (h_value >value)
      {
       h_value-=value; 
      }
     
      Serial.print(h_value);
   h_servo.write(h_value);
   automation =0;
  }
  
  void servo_cam::v_move_right(int value){
     if (v_value <180 -value)
      {
       v_value+=value; 
      }
      Serial.print(v_value);
    v_servo.write(v_value);
    automation =0;
  }
  
  void servo_cam::_move_zero(byte a){
    if ( a == 0 &&  automation ==0){
       automation =1; 
    }
    else if ( a == 1 && automation == 1)  {return;}
    h_value = 70;
    v_value = 90;
    h_servo.write(h_value);
    v_servo.write(v_value);
    
  }
  void servo_cam::h_move_left_turn(){
   if (automation == 0) {return;}
    h_value = 55;
    v_value = 90;
    h_servo.write(h_value);
    v_servo.write(v_value);
  }
  void servo_cam::h_move_right_turn(){
    if (automation == 0) {return;}
    h_value = 85;
    v_value = 90;
    h_servo.write(h_value);
    v_servo.write(v_value);
  }
