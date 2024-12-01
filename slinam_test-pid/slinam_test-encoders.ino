
struct Encoders {
  uint16_t l_pulses;
  uint16_t r_pulses;
};

Encoders enc = { 0 };

float encoder_get_rpm() {
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

  // float l_rps = ((float)l_delt_pulses / ENCODER_SLOT_COUNT) / (float)(millis() - millis_last);
  // float r_rps = ((float)r_delt_pulses / ENCODER_SLOT_COUNT) / (float)(millis() - millis_last);
  float l_rps = ((float)l_delt_pulses / ENCODER_SLOT_COUNT);
  float r_rps = ((float)r_delt_pulses / ENCODER_SLOT_COUNT);

  Serial.print("hello from encoder_get_rpm!");
  // Serial.println(millis() - millis_last);
  Serial.println(l_rps, 6);

  millis_last = millis();

  return l_rps * 60;
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

