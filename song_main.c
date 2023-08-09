#include "lpc24xx.h"
#include "songs.h"

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

    struct tone off_sound[]; 
    struct tone on_sound[]; 
    //int rate = ;            //experiment with different values

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
