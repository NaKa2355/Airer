

//赤外線を受信

int receive_ir(int pin, int16_t buf[], uint16_t buf_len, unsigned long recv_timeout, unsigned long pulse_timeout) {
  uint16_t index = 0;
  bool is_first_time = true;
  unsigned long t = micros();
  unsigned long pulse = 0;
  int pre_state = HIGH;
  int now_state = HIGH;
  
  while(1) {
    now_state = digitalRead(pin);
    if(now_state != pre_state) {
      if(!is_first_time) {
        pulse = micros() - t;
        if(index >= buf_len) return 0;
        if(pulse > INT16_MAX) {
          buf[index] = (pulse / 1000) * -1;
        } else {
          buf[index] = pulse;
        }
        index++;
      }
      t = micros();
      pre_state = now_state;
      is_first_time = false;
    } else {
      if(!is_first_time) {
        if(micros() - t > pulse_timeout) {
          return index;
        }
      } else {
        if(micros() - t > recv_timeout) {
          return 0;
        }
      }
    }
  }
}
