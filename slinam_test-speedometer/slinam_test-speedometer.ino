#include <TimerOne.h>
#include <TimerTwo.h>
#include "slinam_test.h"

#define TEST_RPM 24

struct Encoders {
  uint16_t l_pulses;
  uint16_t r_pulses;
  uint16_t l_delta_pulses;
  uint16_t r_delta_pulses;
};

struct TimeStuff {
  uint32_t millis_last;
  uint32_t millis_now;
  uint32_t millis_delta;
};

volatile Encoders enc = { 0 };
TimeStuff dt = { 0 };

char dbg_buffer[80];

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(MOTOR_A_PINA + i, OUTPUT);
  }

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);

  // Encoder Interrupts
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN),
                  encoder_left_pulse, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN),
                  encoder_right_pulse, CHANGE);

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

  run_motors(TEST_RPM, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
}

unsigned int last_pulses;

void loop() {
  unsigned int delt_pulses = enc.l_pulses - last_pulses;

  Serial.print("left rps: ");
  Serial.println(delt_pulses / ENCODER_SLOT_COUNT);
  Serial.println(delt_pulses);
  Serial.println(enc.l_pulses);

  last_pulses = enc.l_pulses;

  delay(1000);

  // sprintf(dbg_buffer, "millis_delta, _now, _last: %lu, %lu, %lu",
  //         dt.millis_delta, dt.millis_now, dt.millis_last);
  // Serial.println(dbg_buffer);

  // test_calculate_speed();

  // delay(1000);
}
