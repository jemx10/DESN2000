#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "lpc24xx.h"

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
    int lights_manual_overide;
    int blinds_manual_overide;
    int sunset;
    int sunrise;

}

int main() {
    // A continious loop 
    struct manual_input manual_input;
    struct current current;

    while (1) {
        // read the data from the light sensor
        // Itty
        level_from_sensor(&current);

        // read the manual data from the LCD screen
        // Alex and Crystal
        level_from_LCD(&manual_input);

        // Pass in inputs 
        // - light level
        // - manual input
        // Return 
        // - current 
        algorithmn(manual_input, &current);

        // Change LCD screen 
        // Rob
        lcd_output(current);

        // Change tri colour led
        // Alex and Maria
        tricolour_led_output(current);

        // Change LED ladder
        // James
        led_ladder_output(current);

        // Sprinkler 
        // Maria 
        sprinkler(current);

        // On/off sound?
    }
    return 0;
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
    if (manual_input->on_off == 1 && current->on_off == 1) {
        current->on_off = 0;
    } else if (manual_input->on_off == 1 && current->on_off == 0) {
        current->on_off = 1
    };

    // Update day/night
    if (the time is sunrise) {
        current->blinds_manual_overide = FALSE;
        current->lights_manual_overide = FALSE;
        current->day_night = 1;
    } else if (the time is sunset) {
        current->blinds_manual_overide = FALSE;
        current->lights_manual_overide = FALSE;
        current->day_night = 0;
        // close the blinds at sunset
        while (current->blinds > 0) {
            current->blinds--;
            sleep() // time in milliseconds
        }
    }


    // Determine whether the system is on or off
    // System is on
    if (current->on_off == 1) {
        // Determine whether it should act in day or night mode
        // Day mode
        if (current->day_night == 1) {

            // Manual cases blinds
            if (manual_input->blinds_up && current->blinds < 5) {
                current->blinds++;
                current->blinds_manual_overide = TRUE;
            } else if (manual_input->blinds_down && current->blinds > 0) {
                current->blinds--;
                current->blinds_manual_overide = TRUE;
            }
            
            // Manual case lights
            if (manual_input->lights_up && current->lights < 5) {
                current->lights++;
                current->brightness++;
            } else if (manual_input->lights_down && current->lights < 5) {
                current->lights--;
                current->brightness--;
            }

            // Automatic brightness changing
            // House is too bright
            while (light_sensor_level >= brightness) {
                if (current->lights > 0) {
                    current->lights--;
                } else if (current->blinds > 0  && current->blinds_manual_overide == FALSE) {
                    current->blinds--;
                } else {
                    break;
                }
            }
            
            // House is too dark
            while (light_sensor_level >= brightness) {
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
            if (manual_input->blinds_up && current->blinds < 5) {
                current->blinds++;
                current->blinds_manual_overide = TRUE;
            } else if (manual_input->blinds_down && current->blinds > 0) {
                current->blinds--;
                current->blinds_manual_overide = TRUE;
            }

            // Manual case lights
            if (manual_input->lights_up && current->lights < 5) {
                current->lights++;
                current->brightness++;
            } else if (manual_input->lights_down && current->lights < 5) {
                current->lights--;
                current->brightness--;
            }

            // Automatic brightness changing
            // House is too bright
            while (light_sensor_level >= brightness) {
                if (current->lights > 0) {
                    current->lights--;
                } else {
                    break;
                }
            }
            
            // House is too dark
            while (light_sensor_level >= brightness) {
                if (current->lights < 256) {
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
            sleep() // time in milliseconds
        }

        // close the blinds at sunset
        while (current->blinds > 0) {
            current->blinds--;
            sleep() // time in milliseconds
        }
    }
}
