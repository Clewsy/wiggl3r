#include <Arduino.h>

// PIO Home-> Libraries-> Search "Mouse"-> Mouse by arduino-> Add to project.
#include "Mouse.h"

#define LED_PIN 13          // On-board LED.
#define LED_MAX 40          // Maximum value (PWM duty cycle) for LED pulse.
#define LED_FLASH_MS 400    // Duration (ms) LED is at full brightness.

#define PIXEL_DELTA 1       // Number of pixels the cursor will move.
#define TIME_TO_WIGGLE 60   // Numer of seconds before moving the cursor again.
#define ONE_SECOND 15625    // The timer ticks every:
                            // 1/(16MHz/1024(prescaler)) = 0.000064s.  
                            // Therefore need 15625 ticks for one second.

// Function Declarations.
void wiggle_timer_init(void);
void led_pwm_timer_init(void);
void led_pulse(bool enable);
ISR(TIMER3_OVF_vect);
ISR(TIMER1_COMPA_vect);
void wiggle(void);
void setup(void);
void loop(void);
