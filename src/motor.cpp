#include "Arduino.h"
#include "motor.h"
#include "pin_config.h"
#include "bluetooth.h"
#include "line_tracker.h"
#include "distance.h"

#define deadband 20

bool left_motor_fwd = true;
bool right_motor_fwd = true;

bool left_motor_prev = true;
bool right_motor_prev = true;

int left_motor_speed = 0;
int right_motor_speed = 0;

void setup_motors(){  
  //set all pins to output
  pinMode(H_ENA, OUTPUT);
  pinMode(H_IN1, OUTPUT);
  pinMode(H_IN2, OUTPUT);

  pinMode(H_ENB, OUTPUT);
  pinMode(H_IN3, OUTPUT);
  pinMode(H_IN4, OUTPUT);

  digitalWrite(H_IN1,LOW);
  digitalWrite(H_IN2,LOW);
  digitalWrite(H_IN3,LOW);
  digitalWrite(H_IN4,LOW);

  //set default direction to forward
  set_left_forward();
  set_right_forward();
}

void set_left_forward(){
  digitalWrite(H_IN1, LOW);
  digitalWrite(H_IN2, LOW);
  digitalWrite(H_IN1, HIGH);
}

void set_left_reverse(){
  digitalWrite(H_IN1, LOW);
  digitalWrite(H_IN2, LOW);
  digitalWrite(H_IN2, HIGH);
}

void set_right_forward(){
  digitalWrite(H_IN3, LOW);
  digitalWrite(H_IN4, LOW);
  digitalWrite(H_IN3, HIGH);
}

void set_right_reverse(){
  digitalWrite(H_IN3, LOW);
  digitalWrite(H_IN4, LOW);
  digitalWrite(H_IN4, HIGH);
}

void set_motor_directions(){
  //calculate motor speeds using arcade drive equations

  left_motor_speed =  y_axis - x_axis;
  right_motor_speed = y_axis + x_axis;

  // Serial.print(" left : ");
  // Serial.println(left_motor_speed);
  // Serial.print("right : ");
  // Serial.println(right_motor_speed);

  //constrain left and right motor speeds
  left_motor_speed = constrain (left_motor_speed,-100,100);
  right_motor_speed = constrain (right_motor_speed,-100,100);

  //set motor directions
  if (left_motor_speed > 0){
    left_motor_fwd = true;
  } else {
    left_motor_fwd = false;
  }

  if (right_motor_speed > 0){
    right_motor_fwd = true;
  } else {
    right_motor_fwd = false;
  }

  //only write motor directions if direction changes
  if(left_motor_fwd != left_motor_prev){
    if(left_motor_fwd){
      set_left_forward();
    } else {
      set_left_reverse();
    }
  }

  //only write motor directions if direction changes
  if(right_motor_fwd != right_motor_prev){
    if(right_motor_fwd){
      set_right_forward();
    } else {
      set_right_reverse();
    }
  }

  //set previous motor directions
  left_motor_prev = left_motor_fwd;
  right_motor_prev = right_motor_fwd;
}

void driver_control(bool collision_detection){
  //set motor directions
  set_motor_directions();
  //check if the motor speeds are outside the bound of the deadband, if they are write the motor speeds
  //if they are not stop the motors
  if((distance_sensor_reading < 10) && collision_detection && (distance_sensor_reading > 1)){
    set_left_reverse();
    set_right_reverse();
    analogWrite(H_ENA,100);
    analogWrite(H_ENB,100);
    delay(500);
    measure_distance_in_cm();
  } else {

    if((abs(left_motor_speed) > deadband) || (abs(right_motor_speed) > deadband)){
      analogWrite(H_ENA,fabs(left_motor_speed*255.0/100));
      analogWrite(H_ENB,fabs(right_motor_speed*255.0/100));
    } else {
      digitalWrite(H_ENA,LOW);
      digitalWrite(H_ENB,LOW);
    }
  
  }

}