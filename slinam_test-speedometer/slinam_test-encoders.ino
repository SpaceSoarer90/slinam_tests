#include "slinam_test.h"
static uint16_t l_pulses_last, r_pulses_last;
static uint16_t l_pulses_curr, r_pulses_curr;
static uint16_t l_delta_pulses, r_delta_pulses = 0;

// currently this function takes about ~400us
void encoder_get_delta() {
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
                  encoder_left_pulse, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN),
                  encoder_right_pulse, FALLING);
}

// for some reason, adding a delay makes the readings more
// accurate. i guess it's something like "debouncing" the
// readings.
void encoder_left_pulse() {
  enc.l_pulses++;
  // delayMicroseconds(256);
  // delayMicroseconds(1000);
}

void encoder_right_pulse() {
  enc.r_pulses++;
  // delayMicroseconds(256);
  // delayMicroseconds(1000);
  // Serial.print("right pulses: ");
  // Serial.println(enc.r_pulses);
}

void test_calculate_speed() {

  dt.millis_now = millis();
  dt.millis_delta = dt.millis_now - dt.millis_last;
  encoder_get_delta();
  dt.millis_last = dt.millis_now;

  sprintf(dbg_buffer, "dt encoders: %u, %u", enc.l_delta_pulses,
          enc.r_delta_pulses);
  Serial.println(dbg_buffer);

  sprintf(dbg_buffer, "cur encoders: %u, %u", enc.l_pulses,
          enc.r_pulses);
  Serial.println(dbg_buffer);

  sprintf(dbg_buffer, "last encoders: %u, %u", l_pulses_last,
          r_pulses_last);
  Serial.println(dbg_buffer);

  Serial.println();

  // sprintf(dbg_buffer, "speeds: %s, %s", String(l_speed).c_str(), String(r_speed).c_str());
  // Serial.println(dbg_buffer);
  // Serial.println();
}

