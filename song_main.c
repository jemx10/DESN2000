#include "lpc24xx.h"


#define OFF		100

#define C4 		3831
#define CS4 	3610
#define D4 		3401
#define DS4 	3215
#define E4 		3030
#define F4 		2857
#define FS4 	2725
#define G4 		2551
#define GS4 	2410
#define A4 		2272
#define AS4 	2146
#define B4 		2024

#define C5 		C4/2
#define CS5 	CS4/2
#define D5 		D4/2
#define DS5 	DS4/2
#define E5 		E4/2
#define F5 		F4/2
#define FS5 	FS4/2
#define G5 		G4/2
#define GS5 	GS4/2
#define A5 		A4/2
#define AS5 	AS4/2
#define B5 		B4/2

#define C6 		C5/2
#define CS6 	CS5/2
#define D6 		D5/2
#define DS6 	DS5/2
#define E6 		E5/2
#define F6 		F5/2
#define FS6 	FS5/2
#define G6 		G5/2
#define GS6 	GS5/2
#define A6 		A5/2
#define AS6 	AS5/2
#define B6 		B5/2

#define C3 		C4*2
#define CS3 	CS4*2
#define D3 		D4*2
#define DS3 	DS4*2
#define E3 		E4*2
#define F3 		F4*2
#define FS3 	FS4*2
#define G3 		G4*2
#define GS3 	GS4*2
#define A3 		A4*2
#define AS3 	AS4*2
#define B3 		B4*2

struct tone {
	int duration;
	int pitch;
	int volume;
};
int song_duration;
void udelay(unsigned int delay_in_us);
void play_tone(unsigned int duration, int period,  int vol);
void setup_DAC(void);

int main() {
    //note that duration is unitless and is scaled by 'rate'
    //pitch is period in useconds (same as delay)
    //volume ranges from 0 to 0x3FF
    struct tone off_sound[] = {} 
    struct tone on_sound[] = {}
    int rate = ;            //experiment with different values

    int i;

    setup_DAC();

    //not sure how to make this edge triggered at the on/off signal

    if (current->on_off == 0) {
        for (i = 0; i < song_duration; i++) {
            play_tone(rate * on_sound[i].duration, on_sound[i].pitch, on_sound[i].volume);
        }
    }
    else if (current->on_off == 1) {
        for (i = 0; i < song_duration; i++) {
            play_tone(rate * off_sound[i].duration, off_sound[i].pitch, off_sound[i].volume);  
        }
    }

void setup_DAC(void) {
        PINSEL1 |= 0x200000;
        T0PR = 72;     //pre-scaled timer given pclk is 72MHz, and delay in useconds
        T0MCR |= 0x4;  //set timer to stop and disable when delay time is reached
}

//delay function
void udelay(unsigned int delay_in_us) {
    T0TCR |= 0x2; //Reset
    T0TCR &= ~0x2; //clear reset bit
    T0TCR |= 0x1; //re-enable timer
    T0MR0 = delay_in_us; //Add delay time to register
    while(1) {
        if ((T0TCR & 1) == 0) {
            break; //Break out if timer stops
        }
    }
}
//audio output function
void play_tone(unsigned int duration, int period, int vol) {
    int times = duration/period;
    int i = 0;
    for (i = 0; i < times; i++) {
        DACR |= (vol << 6); //Set the volume (pos edge)
        udelay(period/2); 
        DACR &= 0x003F;  //Turn off the output (neg edge)
        udelay(period/2);
    }
}
