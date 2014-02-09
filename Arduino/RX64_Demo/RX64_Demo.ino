/**
 * \author Michael Bartling bart4128@tamu.edu
 * \author Rabih Abifaraj   rabifaraj@tamu.edu
 * \date 11/8/2013
 * \version 1.0
 * 
 * \copyright The MIT License (MIT)
 */


#define BAUD_RATE 115200

/*Declare serial ports */
//Xbee xbee = Xbee();
uint16_t length;
uint8_t  lengthSegHigh;
uint8_t  lengthSegLow;

// We only get one LED to play with
int statusLed 	   = 6;
int errorLed 	   = 6;
uint8_t api_id;
uint8_t api_fd[128];
const int reset_pin = 4;
const int led_pin = 11;  // 11=Teensy 2.0, 6=Teensy 1.0, 16=Benito
const int led_on = HIGH;
const int led_off = LOW;
unsigned long baud = 115200;

HardwareSerial Uart = HardwareSerial();

void simpleXbeeRead();

void setup()
{
  //pinMode(statusLed, OUTPUT);
  //pinMode(errorLed, OUTPUT);
  pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, led_off);
	digitalWrite(reset_pin, HIGH);
	pinMode(reset_pin, OUTPUT);
//  Serial.begin(BAUD_RATE);
//  Uart.begin(BAUD_RATE);
	Serial.begin(BAUD_RATE);	// USB, communication to PC or Mac
	Uart.begin(BAUD_RATE);	// UART, communication to Dorkboard
  //from api_try.ino
  //xbee.setSerial(Uart);
  //xbee.begin(BAUD_RATE)
    Serial.println("Starting the Receiver!");
  Uart.flush();

}

void flashLed(int pin, int times, int wait) {

	for (int i = 0; i < times; i++) {
		digitalWrite(pin, HIGH);
		delay(wait);
		digitalWrite(pin, LOW);

		if (i + 1 < times) {
			delay(wait);
		}
	}
}

 long led_on_time=0;
volatile int enable = 1;
void loop()
{
	//Do other functions here
	//
unsigned char c, dtr;
	static unsigned char prev_dtr = 0;

	if (Serial.available()) {
                enable = 0;
		c = Serial.read();
                if(c == '$') enable = 1;
                if(c == '^') Uart.flush();
                
		Uart.write(c);
		digitalWrite(led_pin, led_on);
		led_on_time = millis();
		return;
	}
if( enable == 1)
{
	
	//simpleXbeeRead();
	//flashLed(statusLed, 1, 100);
	if (Uart.available()) {
		c = Uart.read();
                if(c == 0x7E) Serial.println();
		Serial.print(c,HEX);
		digitalWrite(led_pin, led_on);
		led_on_time = millis();
		return;
	}
}
else if( enable == 0)
{
	if (Uart.available()) {
		c = Uart.read();
		Serial.write(c);
		digitalWrite(led_pin, led_on);
		led_on_time = millis();
		return;
	}
}

	dtr = Serial.dtr();
	if (dtr && !prev_dtr) {
		digitalWrite(reset_pin, LOW);
		delayMicroseconds(250);
		digitalWrite(reset_pin, HIGH);
	}
	prev_dtr = dtr;
	if (millis() - led_on_time > 3) {
		digitalWrite(led_pin, led_off);
	}
	if (Serial.baud() != baud) {
		baud = Serial.baud();
		if (baud == 57600) {
			// This ugly hack is necessary for talking
			// to the arduino bootloader, which actually
			// communicates at 58824 baud (+2.1% error).
			// Teensyduino will configure the UART for
			// the closest baud rate, which is 57143
			// baud (-0.8% error).  Serial communication
			// can tolerate about 2.5% error, so the
			// combined error is too large.  Simply
			// setting the baud rate to the same as
			// arduino's actual baud rate works.
			Uart.begin(58824);
		} else {
			Uart.begin(baud);
		}
	}
}

/**
 * \brief Simple Arduino Xbee Read function
 * Make sure enough data received on buffer and then when a start frame is received (0x7E) begin forming dummy packet.
 *
 *
 */

void simpleXbeeRead()
{
  	
	//Begin Actual read 
	//if(Uart.available() > 20) //Get at least something in the buffer
        if(Uart.available() >= 2)
	{
	  /* 7E is the start frame of our packet */
		if(Uart.read()==0x7E)
		{
			lengthSegHigh = Uart.read();
			lengthSegLow = Uart.read();
			
			/* The length are the first two bytes we read after the start frame */
			length = ((lengthSegHigh<<8) & 0xff00) | ((lengthSegLow) & 0x00ff);
			
			/* The api frame id is the next thing we read after the length */
			api_id = Uart.read();
			
			/*Everything else for the remainder of the length is data */
			for(int i = 0; i < length; i++) /**\todo Make sure this doesnt need to start from 1 instead of 0*/
			{
			        api_fd[i] = Uart.read();
                                //Serial.print(apiFramePtr->api_fd[i]);
                                //Serial.print(" ");
			}
//                        Serial.print('\n');
                        Serial.print("A ");
                        for(int i = 4; i < 7; i++) /**\todo Make sure this doesnt need to start from 1 instead of 0*/
			{
			
                                Serial.print(api_fd[i]);
                                Serial.print(".");
			}
                        Serial.print(api_fd[7]);
                        Serial.print(" ");  //Last part of IP doesnt need .
                        
                        Serial.print("R ");
                        Serial.print(api_fd[8]); //RSSI
                        
                        Serial.println();
		}

                Uart.flush();

	}

}

