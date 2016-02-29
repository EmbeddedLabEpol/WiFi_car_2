#include <Arduino.h>
#include <Servo.h>
class servo_cam {
  public:
  servo_cam();
  ~servo_cam();
  void init ( int v_axis,int h_axis);
  void h_move_left(int value);
  void v_move_left(int value);
  void h_move_right(int value);
  void v_move_right(int value);
  void h_move_left_turn();
  void h_move_right_turn();
  void _move_zero(byte a );
  
  private:
  int h_axis;
  int v_axis;
  byte automation;
  Servo h_servo;
  Servo v_servo;
  volatile unsigned int h_value;
  volatile unsigned int v_value;
};
