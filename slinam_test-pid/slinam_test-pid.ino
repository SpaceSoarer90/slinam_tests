#include <PID_v1.h>

#define MOTOR_A_PWM 9
#define MOTOR_B_PWM 10

#define MOTOR_MAX_RPM 1100

#define MOTOR_A_PINA 4
#define MOTOR_A_PINB 5
#define MOTOR_B_PINA 6
#define MOTOR_B_PINB 7
enum {
  MOTOR_A_RPM,
  MOTOR_B_RPM
};

#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3
#define ENCODER_SLOT_COUNT 20

enum PID_Modes {
  PID_PROPOTIONAL,
  PID_INTEGRAL,
  PID_DERIVATIVE,
};

struct PIDValues {
  double input;
  double output;
  double setpoint;

  double k_p;
  double k_i;
  double k_d;

  int p_on;
  int cont_dir;

  int control_constant_mode;
};

PIDValues pid_l = { 0 };
PID pid_cont_l(&pid_l.input, &pid_l.output, &pid_l.setpoint,
               pid_l.k_p, pid_l.k_i, pid_l.k_d, DIRECT);
PIDValues pid_r = { 0 };
PID pid_cont_r(&pid_r.input, &pid_r.output, &pid_r.setpoint,
               pid_r.k_p, pid_r.k_i, pid_r.k_d, DIRECT);
char dbg_buffer[80];

uint32_t timeout;
uint32_t timeout_enc;

void setup() {
  test_setup_motors();
  test_setup_encoders();

  pid_cont_l.SetMode(AUTOMATIC);
  pid_l.k_p = 2;
  pid_l.k_i = 5;
  pid_l.k_d = 1;

  pid_cont_l.SetTunings(pid_l.k_p, pid_l.k_i, pid_l.k_d);
  pid_cont_l.SetOutputLimits(0, 200);
  // pid_cont_l.SetOutputLimits(-MOTOR_MAX_RPM, MOTOR_MAX_RPM);

  pid_l.input = 0.0;
  pid_l.output = 0.0;
  pid_l.setpoint = 0.0;

  pid_cont_r.SetMode(AUTOMATIC);
  pid_r.k_p = 2;
  pid_r.k_i = 5;
  pid_r.k_d = 1;

  pid_cont_r.SetTunings(pid_r.k_p, pid_r.k_i, pid_r.k_d);
  pid_cont_r.SetOutputLimits(0, 200);
  // pid_cont_r.SetOutputLimits(-MOTOR_MAX_RPM, MOTOR_MAX_RPM);

  pid_r.input = 0.0;
  pid_r.output = 0.0;
  pid_r.setpoint = 0.0;


  Serial.begin(115200);
  Serial.println("SLINAM v1.0: test-pid");
  Serial.println("Testing pid functionality.");


  run_motors(1000, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
}

void loop() {
  if (Serial.available() > 0) {
    char key_press = Serial.read();

    switch (key_press) {
      case 'h':
        pid_l.setpoint = 0.00;
        break;
      case 'j':
        pid_l.setpoint -= 25.0;
        break;
      case 'k':
        pid_l.setpoint += 25.0;
        break;
      case 'l':
        pid_l.setpoint = MOTOR_MAX_RPM;
        break;

      // set mode
      case 'J':
        pid_l.control_constant_mode--;
        break;
      case 'K':
        pid_l.control_constant_mode++;
        break;
      case 'H':
        switch (pid_l.control_constant_mode) {
          case PID_PROPOTIONAL:
            pid_l.k_p -= 0.1;
            break;
          case PID_INTEGRAL:
            pid_l.k_i -= 0.1;
            break;
          case PID_DERIVATIVE:
            pid_l.k_d -= 0.1;
            break;
        }
        break;
      case 'L':
        switch (pid_l.control_constant_mode) {
          case PID_PROPOTIONAL:
            Serial.println("proportinal plus");
            pid_l.k_p += 0.1;
            break;
          case PID_INTEGRAL:
            Serial.println("integral plus");
            pid_l.k_i += 0.1;
            break;
          case PID_DERIVATIVE:
            Serial.println("derivative plus");
            pid_l.k_d += 0.1;
            break;
        }
        break;
    }
    if (pid_l.setpoint < -MOTOR_MAX_RPM) {
      pid_l.setpoint = -MOTOR_MAX_RPM;
    } else if (pid_l.setpoint > MOTOR_MAX_RPM) {
      pid_l.setpoint = MOTOR_MAX_RPM;
    }

    if (pid_l.control_constant_mode < 0) {
      pid_l.control_constant_mode = 0;
    } else if (pid_l.control_constant_mode > PID_DERIVATIVE) {
      pid_l.control_constant_mode = PID_DERIVATIVE;
    }

    pid_cont_l.SetTunings(pid_l.k_p, pid_l.k_i, pid_l.k_d);

    Serial.print("PID Setpoint: ");
    Serial.println(pid_l.setpoint);
    Serial.print("PID Constant mode: ");
    Serial.println(pid_l.control_constant_mode);
    sprintf(dbg_buffer, "(k_p, k_i, k_d): %s, %s, %s",
            String(pid_l.k_p).c_str(), String(pid_l.k_i).c_str(),
            String(pid_l.k_d).c_str());
    Serial.println(dbg_buffer);
  }

  if (millis() - timeout_enc >= 500) {
    Serial.println("updating time");
    pid_l.input = encoder_get_rpm(MOTOR_A_RPM);
    // if (pid_l.output < 0) {
    //   pid_l.input *= -1;
    // }
    timeout_enc = millis();
    pid_cont_l.Compute();
    run_motors((int)pid_l.output, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
  }



  if (millis() - timeout >= 1000) {

    sprintf(dbg_buffer, "(in, out, setp, k_p, k_i, k_d): %s, %s, %s, %s, %s, %s",
            String(pid_l.input).c_str(), String(pid_l.output).c_str(),
            String(pid_l.setpoint).c_str(), String(pid_l.k_p).c_str(),
            String(pid_l.k_i).c_str(), String(pid_l.k_d).c_str());
    // Serial.print("pid states (in, out, setp, kp, ki, kd): ");
    // Serial.print(pid_l.input);
    // Serial.print(pid_l.output);
    // Serial.print(pid_l.setpoint);
    // Serial.print(pid_l.k_p);
    // Serial.print(pid_l.k_i);
    // Serial.println(pid_l.k_d);
    timeout = millis();
    Serial.print(dbg_buffer);
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
