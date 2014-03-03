#include "serial.h" 

int main(){
	char *portname = "/dev/ttyACM0";
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
	        error_message ("error %d opening %s: %s", errno, portname, strerror (errno));
	        return;
	}
	
	set_interface_attribs (fd, B9600, 0);  // set speed to 9600 bps, 8n1 (no parity)
	set_blocking (fd, 0);                // set no blocking

	while(1){

		//read from serial
		char message[100];
		fgets(message, 100, stdin);
		write(fd, message, 100);

		//write to serial
		char buf[100];
		read(fd, buf, sizeof buf);
		printf("%s\n", buf);
	}
}
