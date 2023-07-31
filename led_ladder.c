#include "lpc24xx.h"
#include <stdio.h>

#define all_led 0x1FE
#define enable 0x400000
void setup_ladder(void) {
    FI02DIR |= all_led         //configure led as outputs
    FIO0PIN |= enable          //enable led ladder (P0.22)
}
    
