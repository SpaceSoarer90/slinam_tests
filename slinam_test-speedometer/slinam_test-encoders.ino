#include "slinam_test.h"

// currently this function takes about ~400us
void encoder_get_speed() {
  static int   l_pulses_last, r_pulses_last;
  static int   l_pulses_curr, r_pulses_curr;
  static int   l_delta_pulses, r_delta_pulses = 0;

  detachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN));
  detachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN));

  // i don't like it when there are global variables
  // in the function stuff
  // i know it's slow, just shush pls.
  l_pulses_curr = enc.l_pulses;
  r_pulses_curr = enc.r_pulses;

  l_delta_pulses = l_pulses_curr - l_pulses_last;
  r_delta_pulses = r_pulses_curr - r_pulses_last;

  l_pulses_last = l_pulses_curr;
  r_pulses_last = r_pulses_curr;

  // yes, these can actually NOT exist
  enc.l_delta_pulses = l_delta_pulses;
  enc.r_delta_pulses = r_delta_pulses;

  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN),
                  encoder_left_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN),
                  encoder_right_pulse, RISING);
}

void encoder_left_pulse() {
  enc.l_pulses++;  
  Serial.print("left pulses: ");
  Serial.println(enc.l_pulses);
}

void encoder_right_pulse() {
  enc.r_pulses++;  
  Serial.print("right pulses: ");
  Serial.println(enc.r_pulses);
}
