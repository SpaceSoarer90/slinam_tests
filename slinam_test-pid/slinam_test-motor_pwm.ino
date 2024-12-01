
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
