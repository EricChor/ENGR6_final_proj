#pragma once

extern void setup_accelerometer();

extern void calibrate();

extern void load_calibration_from_mem();

extern void store_calibration_in_mem();

extern void print_calibration_vals();

extern void update_accel_vals();

extern int read_axis();

extern void print_accel_calibration();

extern void compare_to_stored(float x_accel_in, float y_accel_in);

extern void store_accel_to_EEPROM();

extern void print_accel_from_EEPROM();