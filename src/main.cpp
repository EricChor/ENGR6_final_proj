#include <Arduino.h>
#include "bluetooth.h"
#include "motor.h"
#include "line_tracker.h"
#include "accelerometer.h"
#include "distance.h"

void setup() {
  setup_motors();
  setup_bluetooth();
  setup_tracker();
  setup_accelerometer();
  setup_ultrasonic();
  Serial.begin(9600);

  if(false){
    calibrate();
  } else {
    load_calibration_from_mem();
  }
  print_calibration_vals();
}


unsigned long last_distance_read = 0;

void loop() {
  // update_bluetooth();

  if (start_autonomous_pressed){
    check_auton_pins();
    update_tracker_values();
    autonomous_control(50,800,50,800,50,800,300,150,1.2,1.2,3);
  } else {
    driver_control(true);
    update_accel_vals();
    update_bluetooth();

    if(millis()-last_distance_read >= 100){
      measure_distance_in_cm();
      last_distance_read = millis();
    }
  }
  
}

