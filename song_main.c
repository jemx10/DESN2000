#include "lpc24xx.h"
#include "songs.h"

extern struct tone song_data[];
extern int song_duration;
extern void udelay(unsigned int delay_in_us);
extern void play_tone(unsigned int duration, int period,  int vol);
extern void setup_DAC(void);

int song_rate = 52000;


int i;

setup_DAC();

for (i = 0; i < song_duration; i++) {
    


