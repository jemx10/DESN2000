#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "lpc24xx.h"

// Useful #defines 
#define TRUE            1
#define FALSE           0

// define LED masks in rainbow order (7)
#define OFFLIGHT        0
#define REDLIGHT        0x10000
#define YELLOWLIGHT     0x30000
#define GREENLIGHT      0x20000
#define BLUELIGHT       0x40000
#define CYANLIGHT       0x60000
#define MAGENTALIGHT    0x50000
#define WHITELIGHT      0x70000
#define ALLLIGHTS       0x70000

// define all the blind masks for the second LED
#define OFFBLIND        0
#define REDBLIND        0x080000
#define YELLOWBLIND     0x180000
#define GREENBLIND      0x100000
#define BLUEBLIND       0x200000
#define CYANBLIND       0x300000
#define MAGENTABLIND    0x280000
#define WHITEBLIND      0x380000
#define ALLBLINDS       0x380000

// define the led ladder output
#define BLINDS          0xE0   
#define LIGHT_1         0x10
#define LIGHT_2         0x08
#define SPRKINLER       0x04
#define ALLLLADDER      0xFB

// Push Buttons
#define UPBUTTON        0x400
#define DOWNBUTTON        0x800


// Currently our data is in these structs but we might move it to a single 
// integer with each feild contained within a bit
struct manual_input {
    int brightness_up;
    int brightness_down;
    // int blinds_page;
    int blinds_up;
    int blinds_down;
    // int lights_page;
    int lights_up;
    int lights_down;
    // int on/off
    int sprinkler_on_off;
};

struct current {
    int light_sensor_level;
    int brightness;
    int lights;
    int blinds;
    int sprinkler;
    int on_off;
    int page;
    int mode;
    int time; // HMMMM
    int day_night;
    int blinds_manual_overide;
    int sunset;
    int sunrise;
    int light_1;
    int light_2;
};

// 1 clock - time of day -> modify scalar to show the project working in 
// reduced time
// 1 clock used to implement a sleep function e.g time between blind increments

void setup();
int level_from_sensor();
void pushbuttons(struct manual_input *manual_input);
void tricolour_led_output(struct current current);
void led_ladder_output(struct current current);
void algorithmn(struct manual_input manual_input, struct current *current);
void sprinkler();

int main() {
    // A continious loop 
    struct manual_input manual_input;
    struct current current;

    current.on_off = 1;
    current.day_night = 1;
    current.lights = 0;
    current.blinds = 0;
    current.light_1 = 1;
    current.light_2 = 1;

    setup();

    while (1) {
        // read the data from the light sensor
        // Itty
        
        // if (interupt happens)
        // else we won't take in input from the data
        if (((AD0GDR >> 31) & 1) == 1) {
            current.light_sensor_level = level_from_sensor();
        }

        pushbuttons(&manual_input);

        // read the manual data from the LCD screen
        // Alex and Crystal
        // if (fast interupt)
        // level_from_LCD(&manual_input);

        // Pass in inputs 
        // - light level
        // - manual input
        // Return 
        // - current 
        // always runs :)
        algorithmn(manual_input, &current);

        // Change LCD screen 
        // Rob
        // if there has been a change this will update
        // lcd_output(current);

        // Change tri colour led
        // Alex and Crystal
        // if there has been a change this will update
        tricolour_led_output(current);

        // Change LED ladder
        // James
        // if there has been a change this will update
        led_ladder_output(current);

        // Sprinkler 
        // Maria 
        // if the sprinkler timer turns on 
        // sprinkler(current);

        // On/off sound?
    }
    return 0;
}

void pushbuttons(struct manual_input *manual_input) {
    if ((((FIO0PIN & UPBUTTON) >> 10) == 1)) {
        manual_input->brightness_up;
        sleep(1000);
    } else if ((((FIO0PIN & DOWNBUTTON) >> 10) == 1)) {
        manual_input->brightness_down;
        sleep(1000);
    }
}

void setup () {
    AD0CR = 0x1200F02;
    PCONP = 0x1000;
    PINSEL1 &= ~(0x30000);    
    PINSEL1 |= 0x10000;

    // --------- TRICOLOUR LED -----------
    // setup LEDs with FIO2DIR / FIO0PIN / FIO0DIR
    // used the tricolour LEDs (P0.10 and P0.11)
    FIO3DIR |= 0x500;
	FIO3SET = 0;

    // LED Ladder (P2.1 - P2.8)
    // change direction to be output pin and bit clear 
    // to debug just turn on all lights for now
    FIO0DIR = (1 << 22);
    FIO0SET = (1 << 22);
    FIO2DIR = 0xFF;
    FIO2SET = 0xFF; 

    // set PCLKSEL0
    PCLKSEL0 |= 3 << 24;
}

void tricolour_led_output(struct current current) {
    // The LED lights will come on dependant on the light level
    FIO3PIN &= ~(0x70000);
    if (current.lights == 0) {
        FIO3PIN |= OFFLIGHT;
    } else if (current.lights == 1) {
        FIO3PIN |= BLUELIGHT;
    } else if (current.lights == 2) {
        FIO3PIN |= GREENLIGHT;
    } else if (current.lights == 3) {
        FIO3PIN |= YELLOWLIGHT;
    } else if (current.lights == 4) {
        FIO3PIN |= REDLIGHT;
    } 
    // The LED lights will come on dependant on the blind level
    FIO3PIN &= ~(0x380000);
    if (current.blinds == 0) {
        FIO3PIN = OFFBLIND;
    } else if (current.blinds == 1) {
        FIO3PIN |= BLUEBLIND;
    } else if (current.blinds == 2) {
        FIO3PIN |= GREENBLIND;
    } else if (current.blinds == 3) {
        FIO3PIN |= YELLOWBLIND;
    } else if (current.blinds == 4) {
        FIO3PIN |= REDBLIND;
    } 
}

void led_ladder_output(struct current current) {
    FIO2PIN &= ~(ALLLLADDER);
    if (current.light_1 && current.lights > 0) {
        FIO2PIN |= LIGHT_1;
    } 
    if (current.light_2 && current.lights > 0) {
        FIO2PIN |= LIGHT_2;
    } 
    if (current.blinds > 0) {
        FIO2PIN |= BLINDS;
    } 
    if (current.sprinkler == 1) {
        FIO2PIN |= SPRKINLER;
    }
}

// Initialize any required registers for the ADC functionality here
int level_from_sensor() {
    AD0CR = 0x1200F02;
    return (AD0DR0 >> 6) & 0x3FF;
}


/*
Need to define increments/max values of brightness/lights/blinds
Need tp decide on our brightness level and how that will change
Add sleep 
Decide on how long sunrise and sunset should be
Decode on how long on/off timeing should be
Time for sunset sunrise
*/

void algorithmn(struct manual_input manual_input, struct current *current) {
    // Update on/off based upon the manual input
    // if (manual_input.on_off == 1 && current->on_off == 1) {
    //     current->on_off = 0;
    // } else if (manual_input.on_off == 1 && current->on_off == 0) {
    //     current->on_off = 1
    // };

    // Update day/night
    // if (the time is sunrise) {
    //     current->blinds_manual_overide = FALSE;
    //     current->lights_manual_overide = FALSE;
    //     current->day_night = 1;
    // } else if (the time is sunset) {
    //     current->blinds_manual_overide = FALSE;
    //     current->lights_manual_overide = FALSE;
    //     current->day_night = 0;
    //     // close the blinds at sunset
    //     while (current->blinds > 0) {
    //         current->blinds--;
    //         sleep() // time in milliseconds
    //     }
    // }


    // Determine whether the system is on or off
    // System is on
    if (current->on_off == 1) {
        // Determine whether it should act in day or night mode
        // Day mode
        if (current->day_night == 1) {

            // Manual cases blinds
            if (manual_input.blinds_up && current->blinds < 5) {
                current->blinds++;
                current->blinds_manual_overide = TRUE;
            } else if (manual_input.blinds_down && current->blinds > 0) {
                current->blinds--;
                current->blinds_manual_overide = TRUE;
            }
            
            // Manual case lights
            if (manual_input.lights_up && current->lights < 5) {
                current->lights++;
                current->brightness++;
            } else if (manual_input.lights_down && current->lights < 5) {
                current->lights--;
                current->brightness--;
            }

            // Automatic brightness changing
            // House is too bright
            while (current->light_sensor_level >= current->brightness) {
                if (current->lights > 0) {
                    current->lights--;
                } else if (current->blinds > 0  && current->blinds_manual_overide == FALSE) {
                    current->blinds--;
                } else {
                    break;
                }
            }
            
            // House is too dark
            while (current->light_sensor_level >= current->brightness) {
                if (current->blinds < 256 && current->blinds_manual_overide == FALSE) {
                    current->blinds++;
                } else if (current->lights < 256) {
                    current->lights++;
                } else {
                    break;
                }
            }
        }

        // Night mode 
        else {
            // Manual cases blinds
            if (manual_input.blinds_up && current->blinds < 5) {
                current->blinds++;
                current->blinds_manual_overide = TRUE;
            } else if (manual_input.blinds_down && current->blinds > 0) {
                current->blinds--;
                current->blinds_manual_overide = TRUE;
            }

            // Manual case lights
            if (manual_input.lights_up && current->lights < 5) {
                current->lights++;
                current->brightness++;
            } else if (manual_input.lights_down && current->lights < 5) {
                current->lights--;
                current->brightness--;
            }

            // Automatic brightness changing
            // House is too bright
            while (current->light_sensor_level >= current->brightness) {
                if (current->lights > 0) {
                    current->lights--;
                } else {
                    break;
                }
            }
            
            // House is too dark
            while (current->light_sensor_level >= current->brightness) {
                if (current->lights < 4) {
                    current->lights++;
                } else {
                    break;
                }
            }
        }

    }
    // System is off
    else {
        // Turn off the lights
        while (current->lights > 0) {
            current->lights--;
            sleep(1000); // time in milliseconds
        }

        // close the blinds at sunset
        while (current->blinds > 0) {
            current->blinds--;
            sleep(1000); // time in milliseconds
        }
    }
}

