#include <PID_v1.h>

#define MOTOR_A_PWM 9
#define MOTOR_B_PWM 10

#define MOTOR_MAX_RPM 25

#define MOTOR_A_PINA 4
#define MOTOR_A_PINB 5
#define MOTOR_B_PINA 6
#define MOTOR_B_PINB 7

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3
#define ENCODER_SLOT_COUNT 20

struct PIDValues {
  double input;
  double output;
  double setpoint;

  double k_p;
  double k_i;
  double k_d;

  int p_on;
  int cont_dir;
};

PIDValues pid = { 0 };
PID pid_cont(&pid.input, &pid.output, &pid.setpoint,
             pid.k_p, pid.k_i, pid.k_d, DIRECT);
char dbg_buffer[80];

uint32_t timeout;
uint32_t timeout_enc;

void setup() {
  test_setup_motors();
  test_setup_encoders();

  pid_cont.SetMode(AUTOMATIC);
  pid.k_p = 2;
  pid.k_i = 5;
  pid.k_d = 1;

  pid_cont.SetTunings(pid.k_p, pid.k_i, pid.k_d);


  Serial.begin(115200);

  Serial.println("SLINAM v1.0: test-pid");
  Serial.println("Testing pid functionality.");

  pid.input = 0.0;
  pid.output = 0.0;

  run_motors(20, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
}

void loop() {
  if (Serial.available() > 0) {
    char key_press = Serial.read();
    switch (key_press) {
      case 'h':
        pid.setpoint = 0.00;
        break;
      case 'j':
        pid.setpoint -= 0.50;
        break;
      case 'k':
        pid.setpoint += 0.50;
        break;
      case 'l':
        pid.setpoint = MOTOR_MAX_RPM;
        break;
    }
    Serial.print("Motor PWM: ");
    Serial.println(pid.setpoint);
  }

  if (millis() - timeout_enc >= 1000) {
    pid.input = encoder_get_rpm();
    timeout_enc = millis();
  }

  pid_cont.Compute();


  if (millis() - timeout >= 1000) {

    sprintf(dbg_buffer, "pid states (in, out, setp, kp, ki, kd): %s, %s, %s, %s %s",
            String(pid.input).c_str(), String(pid.output).c_str(),
            String(pid.setpoint).c_str(), String(pid.k_p).c_str(),
            String(pid.k_i).c_str(), String(pid.k_d).c_str());
    // Serial.print("pid states (in, out, setp, kp, ki, kd): ");
    // Serial.print(pid.input);
    // Serial.print(pid.output);
    // Serial.print(pid.setpoint);
    // Serial.print(pid.k_p);
    // Serial.print(pid.k_i);
    // Serial.println(pid.k_d);
    timeout = millis();
    Serial.println(dbg_buffer);
    Serial.println();
  }
}

void test_setup_motors() {
  pinMode(MOTOR_A_PINA, OUTPUT);
  pinMode(MOTOR_A_PINB, OUTPUT);
  pinMode(MOTOR_B_PINA, OUTPUT);
  pinMode(MOTOR_B_PINB, OUTPUT);
}

void test_setup_encoders() {
  pinMode(ENCODER_A_PIN, INPUT);
  pinMode(ENCODER_B_PIN, INPUT);

  // Encoder Interrupts
  attachInterrupt(digitalPinToInterrupt(ENCODER_A_PIN),
                  encoder_left_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B_PIN),
                  encoder_right_pulse, RISING);
}
