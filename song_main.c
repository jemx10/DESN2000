#include "lpc24xx.h"
#include "songs.h"

struct tone off_sound[]; 
struct tone on_sound[]; 
int song_duration;
void udelay(unsigned int delay_in_us);
void play_tone(unsigned int duration, int period,  int vol);
void setup_DAC(void);
int main() {

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

