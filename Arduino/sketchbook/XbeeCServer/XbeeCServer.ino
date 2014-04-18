#include <stdio.h>
//#include <uVector.h>
//#include <XbeeS6.h>
#include <string.h>
#include <Time.h>

#include <XbeeS6c.h>
#include <stdio.h>
#define PC(x) printf("%02X ", x)
#define TEENSY(x) Xbee.write(x)
#define DESKTOP 0
#ifdef DESKTOP
#define XBEE(V) PC(V)
#else
#define XBEE(V) TEENSY(V) 
#endif


#define BAUD_RATE 57600


//Tx64Packet tx_packet;

//uVector myvector;
//static uint8_t * tx_buffer1;

HardwareSerial Xbee = HardwareSerial();
unsigned long baud = 57600;
const int reset_pin = 4;
const int led_pin = 11;    // 11=Teensy 2.0, 6=Teensy 1.0, 16=Benito
const int led_on = HIGH;
const int led_off = LOW;

volatile time_t start_time;
time_t current_time;
long previousMillis = 0;
long interval = 1000; //in ms

uint8_t payload[] = {0x15, 0x16};
uint8_t tx_buff[128];
Api_frame packet;
const uint8_t sf = 0x7E;
void escape_chars(uint8_t *tx_buffer, int send_length);

void setup()
{
    //start_time = now();

    pinMode(led_pin, OUTPUT);
    digitalWrite(led_pin, led_off);
    digitalWrite(reset_pin, HIGH);
    pinMode(reset_pin, OUTPUT);
    set_dst_address(0x00000000, 0xC0A80064);
    Serial.begin(BAUD_RATE);
    Serial.println(F("Starting the Receiver!"));
    second(start_time);
    Serial.print(("Starting time: "));
    Serial.println(start_time);

    Xbee.begin(BAUD_RATE);

    Serial.println(F("======================="));
    Serial.println(F("?: for Receive mode HEX"));
    Serial.println(F("#: for Receive mode BYTE (for Config)"));
    Serial.println(F("$: for TX Mode"));
    Serial.println(F("======================="));
    Xbee.flush();				
    
}
//https://www.pjrc.com/teensy/benchmark_usb_serial_receive.html
int j = 0;
int k = 0;
volatile int enable;
long led_on_time=0;
void loop()
{
        int size = sizeof(payload);
	uint16_t length;
	int send_length;
	int i;
	printf("Size of payload is %d\n", size);

	for(i = 0; i < 10; i++)
	{
	length = req_frame(packet, ID_TX64_REQ, payload, size);
	Serial.printf("Length is %d\n", length);
	send_length = prepare2send(tx_buff, packet);
	Serial.printf("Send length is %d\n", send_length);
	Serial.printf("%02X %02X ", sf, length);
	
       	escape_chars(tx_buff, send_length);
	Serial.printf("\n");
	increment_frame(packet);
	}
delay(5000);
//  
//    //static int enable;
//    unsigned char c, dtr;
//    //int serCount = 0; //See https://www.pjrc.com/teensy/td_serial.html
//    static unsigned char prev_dtr = 0;
//    unsigned long currentMillis = millis();
//    if (Serial.available()) {
//        c = Serial.read();
//        if(c == '?') {
//            enable = 3; 
//            return;
//        }
//
//        if(c == '$' ) {
//            enable    =1; 
//            return;
//        }
//        if(c == '#' ) {
//            enable = 2; 
//            return; 
//        }
//        if(c == '^') {
//            Uart.flush(); 
//            return; 
//        }
//        Uart.write(c);
//        digitalWrite(led_pin, led_on);
//        led_on_time = millis();
//        return;
//    }
//    /* Determine Server Configuration*/
//    if(currentMillis-previousMillis > interval)
//    {
//        if(enable == 1)
//        {
//
//            uint16_t length = tx_packet.prepare2send();
//            uint16_t i;
//
//            for(i = 0; i < length; i++)
//            {
//                Uart.write(tx_buffer1[i]);
//            }
//            for(i = 0; i < length; i++)
//            {
//                Serial.print(tx_buffer1[i], HEX); 
//                Serial.print(" ");
//            }
//            Serial.println(' ');
//            j++;
//            tx_packet.clear_payload();
//            tx_packet.push_back( (uint8_t) j);
//            Uart.flush();
//            Serial.println(F("response"));
//            while(Uart.available())
//            {
//
//                Serial.print(Uart.read(),HEX);
//                Serial.print(" ");
//            }
//            Serial.println(F("EOR"));
//
//
//        }
//        //else
//        if(enable == 2) //For command mode use write for repsponse use print
//        {
//            if (Uart.available()) 
//            {
//                c = Uart.read();
//                Serial.write(c);
//                digitalWrite(led_pin, led_on);
//                led_on_time = millis();
//                return;
//            } 
//        }
//        //else 
//        if(enable == 3) //For command mode use write for response use print
//        {
//            if (Uart.available()) 
//            {
//                c = Uart.read();
//                //		Serial.write(c);
//                current_time = now();
//                //Serial.print("current time: ");
//                //Serial.println(current_time);
//                if(c == 0x7E) Serial.println();
//                Serial.print(c, HEX);
//                Serial.print(" ");
//                digitalWrite(led_pin, led_on);
//                led_on_time = millis();
//                return;
//            } 
//        }
//        previousMillis = currentMillis;
//    }
//    
//    /* Control Bootloader*/
//    dtr = Serial.dtr();
//    if (dtr && !prev_dtr) {
//        digitalWrite(reset_pin, LOW);
//        delayMicroseconds(250);
//        digitalWrite(reset_pin, HIGH);
//    }
//    prev_dtr = dtr;
//    if (millis() - led_on_time > 3) {
//        digitalWrite(led_pin, led_off);
//    }
//    if (Serial.baud() != baud) {
//        baud = Serial.baud();
//        if (baud == 57600) {
//            // This ugly hack is necessary for talking
//            // to the arduino bootloader, which actually
//            // communicates at 58824 baud (+2.1% error).
//            // Teensyduino will configure the UART for
//            // the closest baud rate, which is 57143
//            // baud (-0.8% error).    Serial communication
//            // can tolerate about 2.5% error, so the
//            // combined error is too large.    Simply
//            // setting the baud rate to the same as
//            // arduino's actual baud rate works.
//            Uart.begin(58824);
//        } 
//        else {
//            Uart.begin(baud);
//        }
//    }
//


}

//int main()
//{
//	int size = sizeof(payload);
//	uint16_t length;
//	int send_length;
//	int i;
//	printf("Size of payload is %d\n", size);
//
//	for(i = 0; i < 10; i++)
//	{
//	length = req_frame(packet, ID_TX64_REQ, payload, size);
//	printf("Length is %d\n", length);
//	send_length = prepare2send(tx_buff, packet);
//	printf("Send length is %d\n", send_length);
//	printf("%02X %02X ", sf, length);
//	
//       	escape_chars(tx_buff, send_length);
//	printf("\n");
//	increment_frame(packet);
//	}
////	for(i = 0; i < send_length; i++)
////	{
////		printf("%X ", tx_buff[i]);
////	}
//}
void Xbee_send( uint8_t *tx_buffer, int size)
{
	int send_length;
        int length = req_frame(packet, ID_TX64_REQ, payload, size);
	
	Serial.printf("Length is %d\n", length);
	send_length = prepare2send(tx_buff, packet);
	Serial.printf("Send length is %d\n", send_length);
	Serial.printf("%02X %02X ", sf, length);
	
       	escape_chars(tx_buff, send_length);
	Serial.printf("\n");
	increment_frame(packet);
	
}
void escape_chars( uint8_t *tx_buffer, int send_length)
{
	int i;
    uint8_t escape = 0x7D;
	for(i = 0; i < send_length; i++)
	{
		switch(tx_buffer[i])
		{
			case 0x7E:
				XBEE(escape);
				XBEE(tx_buffer[i] ^ 0x20);
				break;
			case 0x7D:
				XBEE(escape);
				XBEE(tx_buffer[i] ^ 0x20);
				break;
			case 0x11:
				XBEE(escape);
				XBEE(tx_buffer[i] ^ 0x20);
				break;
			case 0x13:
				XBEE(escape);
				XBEE(tx_buffer[i] ^ 0x20);
				break;
			default:
			XBEE(tx_buffer[i]);
				break;
		}
	}
}


