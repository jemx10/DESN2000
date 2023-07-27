#include "lpc24xx.h"
#include "songs.h"

extern struct tone off_sound[]; 
extern struct tone on_sound[]; 
extern int song_duration;
extern void udelay(unsigned int delay_in_us);
extern void sound(unsigned int duration, int period,  int vol);
extern void setup_DAC(void);
extern int startup; // representing on/off state, determines when 'sound' is called
                    // assume that when system is turning off, startup is set to low
                    // when turning on, startup set to high, will need to be accounted for in main

int song_rate = (TBC);            //will play around with different values


int i;

setup_DAC();

if (startup == 0) {
    for (i = 0; i < song_duration; i++) {
        sound(rate * on_sound[i].duration, on_sound[i].pitch, on_sound[i].volume);
    }
}
else if (startup == 1) {
    for (i = 0; i < song_duration; i++) {
        sound(rate * off_sound[i].duration, off_sound[i].pitch, off_sound[i].volume);  
    }
}

