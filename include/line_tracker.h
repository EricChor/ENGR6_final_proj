#pragma once

extern void setup_tracker();

extern void update_tracker_values();

extern void print_tracker_values();

extern void autonomous_control(int left_low, int left_high, int mid_low, int mid_high, int right_low, int right_high, int linear_speed, int base_turn_speed, float left_gain, float mid_gain, float right_gain);

extern void set_pins_for_auton();

extern void check_auton_pins();