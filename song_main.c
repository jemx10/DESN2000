#include "lpc24xx.h"
#include "songs.h"

extern struct tone off_sound[]; 
extern struct tone on_sound[]; 
extern int song_duration;
extern void udelay(unsigned int delay_in_us);
extern void play_tone(unsigned int duration, int period,  int vol);
extern void setup_DAC(void);
extern struct current current;

int song_rate = (TBC);            //will play around with different values


int i;

setup_DAC();

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

