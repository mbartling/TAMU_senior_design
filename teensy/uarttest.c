#include <uart.h>
#include <stdio.h>
#include <usb_serial.h>

int main(void)
{
	uint8_t c;

	CPU_PRESCALE(0); //Run at 16 MHz

	uart_init(BAUD_RATE);

	usb_init();

	printf("Starting the example\n");

	while(1)
	{
		//Do stuff here
		if(uart_available())
		{
			c = uart_getchar();
			usb_serial_putchar(c);
		}

		if(usb_serial_available())
		{
			c = usb_serial_getchar();
			uart_putchar(c);
		}

		// Read sensors here and process.

	}
}
