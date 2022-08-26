// ir-spi (ver 0.1.0) 2022/8/11

// 配線
// 13pin: CLK
// 12pin: MISO 
// 11pin: MOSI
// 10pin: CS
// ~~~~~
//  4pin: SPI通信中にHIGH
//  3pin: 赤外線LED
//  2pin: 赤外線受信モジュール


#include <SPI.h>

//--------------------------defines-------------------------------
#define RECV_PIN 2 //赤外線受信モジュール
#define SEND_PIN 3 //赤外線LED
#define BUSY_PIN 4 //BUSY 

#define MILLI_SEC 1000
#define SEC 1000000


  
typedef struct ir_event_handlers {
  void (*send_buffer)(const int16_t data[], uint16_t data_size);
  int (*receive_request)(int16_t buf[], uint16_t buf_size);
} ir_event_handlers_t;


void send_ir_handler(const int16_t data[], uint16_t data_size);
int receive_ir_handler(int16_t buf[], uint16_t buf_size);

const volatile ir_event_handlers_t handlers = {
  send_ir_handler,
  receive_ir_handler,
};


//----------------------------------------------------------------


//--------------------------functions-----------------------------


//赤外線の送信命令が出た時に呼び出される
void send_ir_handler(const int16_t data[], uint16_t data_size) {
  Serial.println("sending...");
  send_ir(SEND_PIN, data, data_size, 2*SEC);
}


//赤外線の受信命令が出た時に呼び出される
int receive_ir_handler(int16_t buf[], uint16_t buf_size) {
  Serial.println("receiving...");
  int data_size = receive_ir(RECV_PIN, buf, buf_size, 5*SEC, SEC);
  Serial.print(data_size); Serial.println(" pulses received");
  return data_size;
}

void setup() {
  Serial.begin(9600);
  SPCR |= bit(SPE); //SPIのモードをslaveにする
  pinMode(MISO, OUTPUT);
  pinMode(RECV_PIN, INPUT);
  pinMode(SEND_PIN, OUTPUT);
  pinMode(BUSY_PIN, OUTPUT);
  digitalWrite(BUSY_PIN, LOW);
  SPI.attachInterrupt();
  spi_setup();
}

 
void loop() {
  spi_event_loop();
}

//----------------------------------------------------------------
