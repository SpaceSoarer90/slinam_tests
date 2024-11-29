#include <TimerOne.h>
#include <TimerTwo.h>
#include "slinam_test.h"

#define TEST_RPM 20

struct Encoders {
  uint16_t l_pulses;
  uint16_t r_pulses;
  uint8_t  l_delta_pulses;
  uint8_t  r_delta_pulses;
};

struct DeltaTime {
  uint32_t millis_last;
  uint32_t millis_now;
  uint32_t millis_delta;
};

volatile Encoders enc = { 0 };
DeltaTime dt = { 0 };

char dbg_buffer[80];

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(MOTOR_A_PINA + i, OUTPUT);
  }

  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);

  // Encoder Interrupts
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN),
                  encoder_left_pulse, FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN),
                  encoder_right_pulse, FALLING);

  // Realization: it's better to just periodically check
  //              how much it has changed and just calculate
  //              the delta time hhaha
  // Timer Interrupts (Speedometer) every 0.5s
  // Timer2.init(500000, encoder_get_speed);
  // Timer2.start();
  // Timer1.initialize(100000); // time until next check for speed (in us)
  // Timer1.attachInterrupt(encoder_get_speed);

  Serial.begin(115200);
  
  Serial.println("SLINAM v1.0: test-speed");
  Serial.println("Testing printing out speed correctly.");

  // run_motors(TEST_RPM, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
  // run_motors(TEST_RPM, MOTOR_B_PINA, MOTOR_B_PINB, MOTOR_B_PWM);
}

void loop() {
  
  // test_calculate_speed();
  delay(1000);
}

void test_calculate_speed() {
  float l_speed = 0;
  float r_speed = 0;

  dt.millis_now = millis();
  dt.millis_delta = dt.millis_now - dt.millis_last;
  encoder_get_speed();
  dt.millis_last = dt.millis_now;

  l_speed = (float)enc.l_delta_pulses / ((float)dt.millis_delta * 1000);
  r_speed = (float)enc.r_delta_pulses / ((float)dt.millis_delta * 1000);

  // sprintf(dbg_buffer, "millis_delta: %d, %d, %d", dt.millis_delta, 
  //         dt.millis_now, dt.millis_last);
  // Serial.println(dbg_buffer);
  Serial.println(dt.millis_now);
  Serial.println(dt.millis_last);
  Serial.println(dt.millis_delta);
  // sprintf(dbg_buffer, "dt encoders: %d, %d", enc.l_delta_pulses, 
  //         enc.r_delta_pulses);
  sprintf(dbg_buffer, "dt encoders: %d, %d", enc.l_pulses, 
          enc.r_pulses);
  Serial.println(dbg_buffer);
  sprintf(dbg_buffer, "speeds: %s, %s", String(l_speed).c_str(), String(r_speed).c_str());
  Serial.println(dbg_buffer);
  Serial.println();
}

void test_timer_cb() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
