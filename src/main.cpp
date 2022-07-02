#include <Arduino.h>
#include "Mouse.h"  //Add from PIO Home -> Libraries -> Search for "Mouse" -> Mouse by arduino -> Add to project.

#define TIME_TO_WIGGLE 60    // Numer of seconds to elapse before moving the cursor again.
#define ONE_SECOND 15625   // Timer ticks every 1/(16MHz/1024(prescaler)) = 0.000064s.  
                            // Therefore need 15625 ticks for one second.

uint8_t global_seconds = 0;

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

void setup() {

    // TCCR :   Timer/Counter Control Register.
    // WGM :    Waveform Generation Mode.
    // CS :     Clock Select.
    TCCR1A = 0;
    TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10); 
    // Setting WGM1(3:0) to 0b0100 puts timer 1 into CTC mode (Clear Timer on Compare match).
    // So TCNT1 (Timer Counter 1) register is cleared when it matches OCR1A register (Output Compare Register A)
    // Setting CS1(2:0) to 0b101 sets the prescaler for Timer/Counter 1 to 1024.

    // OCR :    Output COmpare Register.
    OCR1A = ONE_SECOND;

    // TIMSK : Timer/Counter Mask Register.
    // OCIE : Output Compare Interrupt Enable.
    TIMSK1 = (1<<OCIE1A);

    // Globally enable interrupts.
    sei(); 

    // Initialise the mouse capability using the Arduino Mouse library.
    Mouse.begin();
}

void loop() {
    if (global_seconds >= TIME_TO_WIGGLE) {
        global_seconds = 0;
        wiggle();
    }
    delay(500);
}