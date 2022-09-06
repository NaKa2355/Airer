//赤外線を送信する


#define MICRO_MAX 16383 //delayMicrosecondsが待てる最大の時間(us)


//38kHzのパルス波を生成
int emit_pulse(int pin, unsigned long pulse_width, unsigned long timeout) {
  if(pulse_width > timeout) return 0;
  unsigned long t = micros();
  while(micros() - t < pulse_width) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(8);
    digitalWrite(pin, LOW);
    delayMicroseconds(18);
  }
  return 0;
}



//t(us)秒間待つ
void smart_delay(unsigned long t, unsigned long timeout) {
  if(t > timeout) return;
  if(t > MICRO_MAX) { //
    delay(t / 1000);
  } else {
    delayMicroseconds(t);
  }
}


//赤外線を送信
int send_ir(int pin, int16_t data[], uint16_t data_len, unsigned long timeout) {
  unsigned long pulse_width;
  unsigned long base_time = micros();
  //Serial.println("sending");
  if(data_len % 2 == 0) return 0;
  for(uint16_t i = 0; i < data_len; i++) {
    if(micros() - base_time > timeout) {
      Serial.println("timeout");
      return 0;
    }
    
    if(i % 2 == 0) {
      if(data[i] >= 0) {
        emit_pulse(pin, (unsigned long)(data[i]), timeout);
      } else {
        emit_pulse(pin, (unsigned long)(data[i] * -1) * 1000, timeout);
      }
    } else {
      if(data[i] >= 0) {
        smart_delay((unsigned long)(data[i]), timeout);
      } else {
        smart_delay((unsigned long)(data[i] * -1) * 1000, timeout);
      }
    }
  }
  return 0;
}
