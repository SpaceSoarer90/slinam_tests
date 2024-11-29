#include <TimerOne.h>

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

struct Encoders {
  int l_pulses;
  int r_pulses;
  float l_speed;
  float r_speed;
};

Encoders enc = { 0 };

void setup() {
  // Encoder Interrupts
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN),
                  encoder_left_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN),
                  encoder_right_pulse, RISING);

  // Timer Interrupts (Speedometer) every 0.1s
  Timer1.initialize(100000); // time until next check for speed (in us)
  Timer1.attachInterrupt(encoder_get_speed);

  Serial.begin(115200);
  
  Serial.println("SLINAM v1.0: test-encoders");
  Serial.println("Testing encoder ticking and track speed.");

}

void loop() {
  delay(600);
  Serial.print('[');
  Serial.print((float)millis() / 1000);
  Serial.print("]: ");
  
  Serial.print("left speed, right speed: ");
  Serial.print(enc.l_speed);
  Serial.print(',');
  Serial.println(enc.r_speed);
}

void encoder_get_speed() {

  static int   l_pulses_last, r_pulses_last;
  static int   l_pulses_curr, r_pulses_curr;
  static int   l_delta_pulses, r_delta_pulses = 0;
  static float l_speed, r_speed = 0;

  Timer1.detachInterrupt();
  detachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN));
  detachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN));

  // i don't like it when there are global variables
  // in the function stuff
  // i know it's slow, just shush pls.
  l_pulses_curr = enc.l_pulses;
  r_pulses_curr = enc.r_pulses;

  l_delta_pulses = l_pulses_curr - l_pulses_last;
  r_delta_pulses = r_pulses_curr - r_pulses_last;

  l_speed = (float)l_delta_pulses;
  r_speed = (float)r_delta_pulses;

  l_pulses_last = l_pulses_curr;
  r_pulses_last = r_pulses_curr;

  enc.l_speed = l_speed;
  enc.r_speed = r_speed;

  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN),
                  encoder_left_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN),
                  encoder_right_pulse, RISING);
  Timer1.attachInterrupt(encoder_get_speed);
}

void encoder_left_pulse() {
  enc.l_pulses++;  
  // Serial.print("left pulses: ");
  // Serial.println(enc.l_pulses);
}

void encoder_right_pulse() {
  enc.r_pulses++;  
  // Serial.print("right pulses: ");
  // Serial.println(enc.r_pulses);
}
