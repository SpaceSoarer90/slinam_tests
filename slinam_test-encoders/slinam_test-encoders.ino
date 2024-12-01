#include <TimerOne.h>

#define ENCODER_SLOT_COUNT 20
#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

#define MOTOR_A_PWM 9
#define MOTOR_B_PWM 10

#define MOTOR_A_PINA 4
#define MOTOR_A_PINB 5
#define MOTOR_B_PINA 6
#define MOTOR_B_PINB 7

struct Encoders {
  uint16_t l_pulses;
  uint16_t r_pulses;
};

Encoders enc = { 0 };

char dbg_buffer[80];

byte motor_pwm = 0;

void setup() {

  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);

  // Encoder Interrupts
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN),
                  encoder_left_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN),
                  encoder_right_pulse, RISING);

  // // Timer Interrupts (Speedometer) every 0.1s
  // Timer1.initialize(100000);  // time until next check for speed (in us)
  // Timer1.attachInterrupt(encoder_get_speed);

  Serial.begin(115200);

  Serial.println("SLINAM v1.0: test-encoders");
  Serial.println("Testing encoder ticking and track speed.");

  test_setup_motors();
  test_run_motors();
}


uint32_t timeout;
int minus_factor;

void loop() {

  if (Serial.available() > 0) {
    char key_press = Serial.read();
    switch (key_press) {
      case 'h':
        motor_pwm = 0;
        break;
      case 'j':
        motor_pwm--;
        break;
      case 'k':
        motor_pwm++;
        break;
      case 'l':
        motor_pwm = 255;
        break;
      case 'J':
        minus_factor--;
        break;
      case 'K':
        minus_factor++;
        break;
    }
    Serial.print("Motor PWM: ");
    Serial.println(motor_pwm);
    test_run_motors();
  }
  // delay(600);
  // Serial.print('[');
  // Serial.print((float)millis() / 1000);
  // Serial.print("]: ");

  // Serial.print("left speed, right speed: ");
  // Serial.print(enc.l_speed);
  // Serial.print(',');
  // Serial.println(enc.r_speed);
  if (millis() - timeout >= 1000) {
    encoder_get_speed();
    timeout = millis();
  }
}

void encoder_get_speed() {
  static uint32_t millis_last;

  static uint16_t l_curr_pulses, r_curr_pulses;
  static uint16_t l_last_pulses, r_last_pulses;
  uint16_t l_delt_pulses, r_delt_pulses;


  detachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN));
  detachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN));
  l_curr_pulses = enc.l_pulses;
  r_curr_pulses = enc.r_pulses;
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN),
                  encoder_left_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN),
                  encoder_right_pulse, RISING);

  l_delt_pulses = l_curr_pulses - l_last_pulses;
  r_delt_pulses = r_curr_pulses - r_last_pulses;

  l_last_pulses = l_curr_pulses;
  r_last_pulses = r_curr_pulses;

  float sec_curr = (float)millis() / 1000.00;
  float sec_last = (float)millis_last / 1000.00;
  float sec_delt = (float)(millis() - millis_last);
  float l_speed = (float)l_delt_pulses / ENCODER_SLOT_COUNT;
  float r_speed = (float)r_delt_pulses / ENCODER_SLOT_COUNT;

  // sprintf(dbg_buffer, "sec_curr, _last, _delta, %d, %d, %s",
  //         String(sec_curr, 6).c_str(),
  //         String(sec_last, 6).c_str(),
  //         String(sec_delt, 6).c_str());
  // Serial.println(dbg_buffer);

  sprintf(dbg_buffer, "left  rps, rpm: %s, %s",
          String(l_speed).c_str(), String(l_speed * 60).c_str());
  Serial.println(dbg_buffer);

  sprintf(dbg_buffer, "right rps, rpm: %s, %s",
          String(r_speed).c_str(), String(r_speed * 60).c_str());
  Serial.println(dbg_buffer);

  sprintf(dbg_buffer, "minus facto: %d", minus_factor);
  Serial.println(dbg_buffer);

  Serial.println();

  millis_last = millis();
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

void test_setup_motors() {
  pinMode(MOTOR_A_PINA, OUTPUT);
  pinMode(MOTOR_A_PINB, OUTPUT);
  pinMode(MOTOR_B_PINA, OUTPUT);
  pinMode(MOTOR_B_PINB, OUTPUT);
}

void test_run_motors() {

  analogWrite(MOTOR_A_PWM, motor_pwm);
  analogWrite(MOTOR_B_PWM, motor_pwm - minus_factor);

  digitalWrite(MOTOR_A_PINA, HIGH);
  digitalWrite(MOTOR_A_PINB, LOW);
  digitalWrite(MOTOR_B_PINA, LOW);
  digitalWrite(MOTOR_B_PINB, HIGH);
}
