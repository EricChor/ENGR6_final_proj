#pragma once

extern void setup_motors();
extern void set_left_forward();
extern void set_left_reverse();
extern void set_right_forward();
extern void set_right_reverse();
extern void set_motor_directions();
extern void driver_control(bool collision_detection);
