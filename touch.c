#include "lpc24xx.h"

#define CS_PIN            0x00100000        //P0.20

void read_touch_input(char *x, char *y);

static unsigned char touch_read(unsigned char command);
void touch_read_xy(char *x, char* y);
void touch_init(void);

void touch_init(void)
{
	//Implement this as you see fit
	//Remember to setup CS_TP as a GPIO output
    FIO0DIR |= CS_PIN;
    FIO0DIR |= 0x48000;
    S0SPCR |= 0x093C;
    S0SPCCR |= 0x24;
}
	

void touch_read_xy(char *x, char* y)
{
	//Read X co-ordinate from the touch screen controller
	*x = touch_read(0xD8);
	//Read Y co-ordinate from the touch screen controller
	*y = touch_read(0x98);
	
}
//separate scaling func for x and y cos not sure how to differentiate between them conditionally lmao
int scale_x(char *x) {
	return x *240/255;
}
int scale_y(char *y) {
	return y *320/255;
}

//note pressure algorithm is x* (z_2/z_1 - 1), feel free to scale as you like
void pressure_command(char* x, char *z_1, char *z_2) {
    *x = touch_read(0xD8);
    *z_1 = touch_read(0xB8);
    *z_2 = touch_read(0xC8);
}

static unsigned char touch_read(unsigned char command)
{
	unsigned short result;
    FIO0CLR |= CS_PIN;
	//Set CS_TP pin low to begin SPI transmission
	
	
	//Transmit command byte on MOSI, ignore MISO (full read write cycle)
	S0SPDR = command;
    while ((S0SPSR >> 7) != 1) {
    }
    
	
	//Transmit 0x00 on MOSI, read in requested result on MISO (another full read write cycle)
    S0SPDR = 0x00;
    while((S0SPSR >> 7) != 1) {
    }
    result = S0SPDR;
	
	FIO0SET |= CS_PIN;
	//Transmission complete, set CS_TP pin back to high
	
	
	//Return 8 bit result.
	return (unsigned char) result;
}	

