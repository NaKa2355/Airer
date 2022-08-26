const volatile int16_t ir_data_buf_size = 600;
const volatile int16_t buf_size = ir_data_buf_size*2 + 8;
volatile uint8_t buf[1208];
volatile int8_t addr_p = 0;
volatile int16_t addr = 0;


//spi通信が行われていないときにtrueを返す
boolean is_spi_idling() {
  if(PINB >> 2 & 1 == 1) {
    return true;
  }
  return false;
}


void spi_setup() {
  memset(buf, 0, buf_size);
  buf[0] = ((uint8_t*)&buf_size)[0];
  buf[1] = ((uint8_t*)&buf_size)[1];
  buf[2] = ((uint8_t*)&ir_data_buf_size)[0];
  buf[3] = ((uint8_t*)&ir_data_buf_size)[1];
}


void send_ir_data() {
  uint16_t data_size = *(uint16_t*)&buf[0x04];
  Serial.println("send");
  Serial.println(data_size);
  if(data_size <= ir_data_buf_size) {
    digitalWrite(BUSY_PIN, HIGH);
    SPI.detachInterrupt();
    Serial.println("send mode");
    Serial.println(data_size);
    handlers.send_buffer((uint16_t*)&buf[0x08], data_size);
    SPI.attachInterrupt();
    digitalWrite(BUSY_PIN, LOW);
  }
}


void receive_ir_data() {
  digitalWrite(BUSY_PIN, HIGH);
  SPI.detachInterrupt();
  Serial.println("recv mode");
  uint16_t data_size = handlers.receive_request((int16_t*)&buf[0x08], ir_data_buf_size);
  *(uint16_t*)(&buf[0x04]) = data_size;
  SPI.attachInterrupt();
  digitalWrite(BUSY_PIN, LOW);
  return;
}


void spi_event_loop() {
  if(is_spi_idling()) {
    addr_p = 0;
    addr = 0;
    switch(buf[0x06]){
      case 0xaa:
        send_ir_data();
        break;
      case 0xff:
        receive_ir_data();
        break;
      default:
        break;
    }
    buf[0x06] = 0;
  }
}


//SPIの割り込み関数
ISR(SPI_STC_vect) {
  if(addr_p < 2) {
    ((int8_t*)&addr)[addr_p] = SPDR;
    addr_p += 1;
  } else {
    if(addr < 0) {
      //write mode
      addr = addr ^ 0x8000;
      if((0x00 <= addr && addr <= 0x03) || addr >= buf_size) {
        addr = addr ^ 0x8000;
        return;
      }
      buf[addr] = SPDR;
      addr += 1;
      addr = addr ^ 0x8000;
    } else if(addr >= 0) {
      //read mode
      if(addr >= buf_size) {
        SPDR = 0;
        return;
      }
      SPDR = buf[addr];
      addr += 1;
    }
  }
}
