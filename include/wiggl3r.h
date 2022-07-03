#include <Arduino.h>
#include "Mouse.h"  //Add from PIO Home -> Libraries -> Search for "Mouse" -> Mouse by arduino -> Add to project.

#define TIME_TO_WIGGLE 60   // Numer of seconds to elapse before moving the cursor again.
#define ONE_SECOND 15625    // Timer ticks every 1/(16MHz/1024(prescaler)) = 0.000064s.  
                            // Therefore need 15625 ticks for one second.

#define LED_MAX 40