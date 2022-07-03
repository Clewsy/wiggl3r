#include <Arduino.h>
#include "Mouse.h"          // Add from PIO Home -> Libraries -> Search for "Mouse" -> Mouse by arduino -> Add to project.

#define LED_PIN 13          // On-board LED.
#define LED_MAX 40          // Maximum analog value (i.e. PWM duty cycle) for LED pulse effect.
#define LED_FLASH_MS 400    // Duration (milliseconds) the LED is at full brightness to signal a cursor move.

#define TIME_TO_WIGGLE 60   // Numer of seconds to elapse before moving the cursor again.
#define ONE_SECOND 15625    // Timer ticks every 1/(16MHz/1024(prescaler)) = 0.000064s.  
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
