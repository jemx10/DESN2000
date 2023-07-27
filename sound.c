#include "lpc24xx.h"

//setup DAC

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
