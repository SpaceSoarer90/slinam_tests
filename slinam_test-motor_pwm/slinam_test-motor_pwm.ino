#define TEST_DELAY 1000

#define MOTOR_MAX_RPM 24

#define MOTOR_A_PWM   9
#define MOTOR_B_PWM   10

#define MOTOR_A_PINA  4
#define MOTOR_A_PINB  5
#define MOTOR_B_PINA  6
#define MOTOR_B_PINB  7


// #define MOTOR_A_PINA  6
// #define MOTOR_A_PINB  7
// #define MOTOR_B_PINA  8
// #define MOTOR_B_PINB  9

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(MOTOR_A_PINA + i, OUTPUT);
  }

  Serial.begin(115200);
  Serial.println("SLINAM v1.0: test-motor_pwm");
  Serial.println("Testing Motor movement (cw and ccw)");
}

void loop() {

  Serial.println("Motor A (left) is running clockwise...");
  for (int i = 1; i <= 4; i++) {
    Serial.print("\tcurrent speed:");
    Serial.println(MOTOR_MAX_RPM / i);
    run_motors(MOTOR_MAX_RPM / i, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);

    delay(TEST_DELAY);
  }

  Serial.println("Motor A (left) is running counter-clockwise...");
  for (int i = 1; i <= 4; i++) {
    Serial.print("\tcurrent speed:");
    Serial.println(MOTOR_MAX_RPM / i);
    run_motors(-MOTOR_MAX_RPM / i, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);

    delay(TEST_DELAY);
  }

  Serial.println("Stopping...");
  run_motors(0, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);

  /*************************************************
   *                                               *
   *                                               *
   *************************************************/

  Serial.println("Motor B (right) is running clockwise...");
  for (int i = 1; i <= 4; i++) {
    Serial.print("\tcurrent speed:");
    Serial.println(MOTOR_MAX_RPM / i);
    run_motors(MOTOR_MAX_RPM / i, MOTOR_B_PINA, MOTOR_B_PINB, MOTOR_B_PWM);

    delay(TEST_DELAY);
  }


  Serial.println("Motor B (right) is running counter-clockwise...");
  for (int i = 1; i <= 4; i++) {
    Serial.print("\tcurrent speed:");
    Serial.println(MOTOR_MAX_RPM / i);
    run_motors(-MOTOR_MAX_RPM / i, MOTOR_B_PINA, MOTOR_B_PINB, MOTOR_B_PWM);

    delay(TEST_DELAY);
  }

  Serial.println("Stopping...");
  run_motors(0, MOTOR_B_PINA, MOTOR_B_PINB, MOTOR_B_PWM);

  /*************************************************
   *                                               *
   *                                               *
   *************************************************/

  Serial.println("Motor A and B are running clockwise...");
  for (int i = 1; i <= 4; i++) {
    Serial.print("\tcurrent speed:");
    Serial.println(MOTOR_MAX_RPM / i);
    run_motors(MOTOR_MAX_RPM / i, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
    run_motors(MOTOR_MAX_RPM / i, MOTOR_B_PINA, MOTOR_B_PINB, MOTOR_B_PWM);

    delay(TEST_DELAY);
  }

  Serial.println("Motor A and B are running counter-clockwise...");
  for (int i = 1; i <= 4; i++) {
    Serial.print("\tcurrent speed:");
    Serial.println(MOTOR_MAX_RPM / i);
    run_motors(-MOTOR_MAX_RPM / i, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
    run_motors(-MOTOR_MAX_RPM / i, MOTOR_B_PINA, MOTOR_B_PINB, MOTOR_B_PWM);

    delay(TEST_DELAY);
  }

  Serial.println("Motor A and B are running opposite of each other...");
  for (int i = 1; i <= 4; i++) {
    Serial.print("\tcurrent speed:");
    Serial.println(MOTOR_MAX_RPM / i);
    run_motors(MOTOR_MAX_RPM / i, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
    run_motors(-MOTOR_MAX_RPM / i, MOTOR_B_PINA, MOTOR_B_PINB, MOTOR_B_PWM);

    delay(TEST_DELAY);
  }

  Serial.println("Motor A and B are running opposite of each other (another time)...");
  for (int i = 1; i <= 4; i++) {
    Serial.print("\tcurrent speed:");
    Serial.println(MOTOR_MAX_RPM / i);
    run_motors(-MOTOR_MAX_RPM / i, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
    run_motors(MOTOR_MAX_RPM / i, MOTOR_B_PINA, MOTOR_B_PINB, MOTOR_B_PWM);

    delay(TEST_DELAY);
  }

  run_motors(0, MOTOR_A_PINA, MOTOR_A_PINB, MOTOR_A_PWM);
  run_motors(0, MOTOR_B_PINA, MOTOR_B_PINB, MOTOR_B_PWM);

  Serial.println("Testing Done!");

  delay(5000);
}

// assumes 8-bit resolution for pwm
uint8_t rpm_to_pwm(int rpm) {
  byte pwm = abs((float(rpm) / MOTOR_MAX_RPM) * 255);
  return pwm;
}

void run_motors(int rpm, uint8_t pinA, uint8_t pinB, uint8_t pinPWM) {
  
  /**
     pinA  | pinB 
       0   |   0    -- TURN OFF
       0   |   1    -- CLOCKWISE
       1   |   0    -- ANTI-CLOCKWISE
       1   |   1    -- SHORT BRAKE (unused)
  */
  // CLOCKWISE - (negative angles are clockwise)
  
  analogWrite(pinPWM, rpm_to_pwm(rpm));
  
  if (rpm < 0) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
  // ANTI-CLOCKWISE - (positive angles are anti-clockwise)
  } else if (rpm > 0) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
  } else {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }
}
