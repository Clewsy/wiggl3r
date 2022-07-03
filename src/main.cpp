#include "wiggl3r.h"

//#include <Arduino.h>
//#include "Mouse.h"  //Add from PIO Home -> Libraries -> Search for "Mouse" -> Mouse by arduino -> Add to project.

//#define TIME_TO_WIGGLE 60   // Numer of seconds to elapse before moving the cursor again.
//#define ONE_SECOND 15625    // Timer ticks every 1/(16MHz/1024(prescaler)) = 0.000064s.  
//                            // Therefore need 15625 ticks for one second.
#define LED_PIN 13
uint8_t global_seconds = 0;

void init_wiggle_timer(void) {
    // TCCR :   Timer/Counter Control Register.
    // WGM :    Waveform Generation Mode.
    // CS :     Clock Select.
    TCCR1A = 0;
    TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10); 
    // Setting WGM1(3:0) to 0b0000 puts timer 1 into CTC mode (Clear Timer on Compare match).
    // So TCNT1 (Timer Counter 1) register is cleared when it matches OCR1A register (Output Compare Register A).
    // Setting CS1(2:0) to 0b101 sets the prescaler for Timer/Counter 1 to 1024.

    // OCR :    Output Compare Register.
    OCR1A = ONE_SECOND;

    // TIMSK : Timer/Counter Mask Register.
    // OCIE : Output Compare Interrupt Enable.
    TIMSK1 = (1<<OCIE1A);
}

void init_led_pwm_timer(void) {
    // TCCR :   Timer/Counter Control Register.
    // WGM :    Waveform Generation Mode.
    // CS :     Clock Select.
    TCCR3A = 0;

    TCCR3B = (1<<CS31); 
    // Setting WGM3(3:0) to 0b0000 puts timer 3 into Normal mode (Clear Timer at max count 0xFFFF).
    // So TCNT3 (Timer Counter 3) register is cleared when it matches max value of 0XFFFF.
    // Setting CS3(2:0) to 0b010 sets the prescaler for Timer/Counter 3 to 8.
    // Therefore counter increments at 16MHz/8 and overflows (triggers interrupt) at (16MHz/8)/0xFFFF ~ 30.5Hz.
   
}

void led_pulse(bool enable) {

    analogWrite(LED_PIN, 0);
    if(enable)  TIMSK3 |= (1<<TOIE3);
    else        TIMSK3 &= ~(1<<TOIE3);
    // TIMSK : Timer/Counter Mask Register.
    // OCIE : Output Compare Interrupt Enable.
    
}


ISR(TIMER3_OVF_vect) {
    static uint8_t led_value = 0;
    static int8_t led_direction = -1;

    if((led_value == 0) | (led_value == LED_MAX)) {
        led_direction = (led_direction ^ -1) + 1;
    }

    led_value = (led_value + led_direction);
    analogWrite(LED_PIN, led_value);
}


ISR(TIMER1_COMPA_vect) {
    global_seconds++;
}

void wiggle(void) {
    static int8_t pixels = 1;

    // Value of pixels becomes negative of previous value.
    pixels = (pixels ^ -1) + 1;

    // Move the cursos "pixels" in the y-axis.
    Mouse.move(0, pixels, 0);
}

void setup(void) {

    pinMode(LED_PIN,OUTPUT);
    
    init_wiggle_timer();

    init_led_pwm_timer();
    led_pulse(true);

    // Globally enable interrupts.
    sei(); 

    // Initialise the mouse capability using the Arduino Mouse library.
    Mouse.begin();
}

void loop(void) {
    if (global_seconds >= TIME_TO_WIGGLE) {
        led_pulse(false);
        digitalWrite(LED_PIN,LOW);
        global_seconds = 0;
        wiggle();
        digitalWrite(LED_PIN,HIGH);
        delay(500);
        led_pulse(true);
    }
    delay(500);
}
