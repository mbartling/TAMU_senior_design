#include <mavlink.h>

// This line defines a "Uart" object to access the serial port
HardwareSerial Uart = HardwareSerial();

void setup() {
Serial.begin(57600);
        Uart.begin(57600);
}

void loop() {
  int incomingByte;
  if (Uart.available() > 0) {
    incomingByte = Uart.read();
    Serial.print("UART received: ");
    Serial.println(incomingByte, DEC);
    //Uart.print("UART received:");
    //Uart.println(incomingByte, DEC);
  }
}

