#include "line_tracker.h"
#include "pin_config.h"
#include "Arduino.h"

int left_tracker_val = 0;
int middle_tracker_val = 0;
int right_tracker_val = 0;
bool auton_pins_set = false;

void setup_tracker(){
    pinMode(left_tracker,INPUT);
    pinMode(middle_tracker,INPUT);
    pinMode(right_tracker,INPUT);
}

void print_tracker_values(){
    Serial.print("left:");
    Serial.print(left_tracker_val);
    Serial.print("|middle:");
    Serial.print(middle_tracker_val);
    Serial.print("|right:");
    Serial.println(right_tracker_val);
}

void update_tracker_values(){
    left_tracker_val = analogRead(left_tracker);
    middle_tracker_val = analogRead(middle_tracker);
    right_tracker_val = analogRead(right_tracker);

    print_tracker_values();
}

int left_sees_line = 0;
int mid_sees_line = 0;
int right_sees_line = 0;
int auton_left_motor_speed = 0;
int auton_right_motor_speed = 0;
void autonomous_control(int left_low, int left_high, int mid_low, int mid_high, int right_low, int right_high, int linear_speed, int base_turn_speed, float left_gain, float mid_gain, float right_gain){
    

    if(abs(left_tracker_val-left_low) < abs(left_tracker_val-left_high)){
        left_sees_line = 0;
    } else {
        left_sees_line = 1;
    }

    if(abs(middle_tracker_val-mid_low) < abs(middle_tracker_val-mid_high)){
        mid_sees_line = 0;
    } else {
        mid_sees_line = 1;
    }

    if(mid_sees_line){
        mid_gain = 0.75;
    }


    if(abs(right_tracker_val-right_low) < abs(right_tracker_val-right_high)){
        right_sees_line = 0;
    } else {
        right_sees_line = 1;
    }

    // Serial.print("left: ");
    // Serial.print(left_sees_line);
    // Serial.print("| mid: ");
    // Serial.print(mid_sees_line);
    // Serial.print("right: ");
    // Serial.print(right_sees_line);


    // Serial.print(left_sees_line);
    // Serial.print(base_turn_speed);
    // Serial.print(left_gain);
    // Serial.println(mid_gain);

    if((left_sees_line) || (right_sees_line)){
        linear_speed = 0;
    }


    auton_right_motor_speed = linear_speed + (right_sees_line*base_turn_speed*right_gain*mid_gain);
    auton_left_motor_speed = linear_speed + (left_sees_line*base_turn_speed*left_gain*mid_gain);

    auton_right_motor_speed = constrain(auton_right_motor_speed,0,1023);
    auton_left_motor_speed = constrain(auton_left_motor_speed,0,1023);

    auton_left_motor_speed = 255.0*auton_left_motor_speed/1023.0;
    auton_right_motor_speed = 255.0*auton_right_motor_speed/1023.0;

    // Serial.print("auto left: ");
    // Serial.print(auton_left_motor_speed);
    // Serial.print("auto right: ");
    // Serial.println(auton_right_motor_speed);

    analogWrite(H_ENA,fabs(auton_left_motor_speed));
    analogWrite(H_ENB,fabs(auton_right_motor_speed));
    
}

void set_pins_for_auton(){
    digitalWrite(H_IN1, LOW);
    digitalWrite(H_IN2, LOW);
    digitalWrite(H_IN1, HIGH);

    digitalWrite(H_IN3, LOW);
    digitalWrite(H_IN4, LOW);
    digitalWrite(H_IN3, HIGH);
}

void check_auton_pins(){
    if(!auton_pins_set){
        set_pins_for_auton();
        auton_pins_set = true;
    }
}