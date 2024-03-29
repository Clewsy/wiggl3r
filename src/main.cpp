#include "wiggl3r.h"

uint8_t global_seconds = 0; // Used to count the number of elapsed seconds.
                            // Global as it is needed in loop() and an ISR.


// Configure a one-second timer.  The timer will trigger an interrupt that will
// count seconds.  Once a defined number of seconds has elapsed, the wiggle()
// function will be called. 
void wiggle_timer_init(void) {

    // TCCR: Timer/Counter Control Registers.
    // WGM: Waveform Generation Mode.
    // CS: Clock Select (prescaler)
    TCCR1A = 0;
    TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);

    // OCR: Output Compare Register.
    OCR1A = ONE_SECOND;

    // TIMSK: Timer/Counter Mask Register.
    TIMSK1 = (1<<OCIE1A);   // OCIE : Output Compare Interrupt Enable.
}

// Configure a timer that will trigger an interrupt to vary the PWM signal to
// the LED.  This is used to vary the LED brightness and create a pulsing
// effect.  Note, this function does not enable the timer.  The timer is enabled
// by the led_pulse() function.
void led_pwm_timer_init(void) {

    // TCCR: Timer/Counter Control Registers.
    TCCR3A = 0;
    TCCR3B = (1<<CS31);     // CS: Clock Select (prescaler).
}

// Turn on/off the LED pulse effect by enabling/disabling the timer interrupt.
void led_pulse(bool enable) {

    analogWrite(LED_PIN, 0);    // Turn the LED off first. 

    // TIMSK: Timer/Counter Mask Register.
    // TOIE: Timer Overflow Interrupt Enable.
    if(enable)  TIMSK3 |= (1<<TOIE3);
    else        TIMSK3 &= ~(1<<TOIE3);
}

// This interrupt is triggered by the timer to control the LED pulse effect.
ISR(TIMER3_OVF_vect) {

    static uint8_t led_value = 0;

    // -1:Brightness decreasing, +1:Brightness increasing. 
    static int8_t led_direction = -1;

    // Reverse led_direction at either end of the brightness range.
    if((led_value == 0) | (led_value == LED_MAX)) {
        led_direction = (led_direction ^ -1) + 1;
    }

    // Update the LED brightness.
    led_value = (led_value + led_direction);
    analogWrite(LED_PIN, led_value);
}

// This interrupt is triggered by a timer every second to count seconds.
ISR(TIMER1_COMPA_vect) {

    global_seconds++;
}

// This function moves the mouse cursor in the Y-axis, alternating direction
// every time it is called.
void wiggle(void) {

    static int8_t pixels = PIXEL_DELTA;

    // Value of pixels becomes negative of previous value.
    pixels = (pixels ^ -1) + 1;

    // Move the cursor "pixels" in the y-axis.
    Mouse.move(0, pixels, 0);
}

void setup(void) {

    // Set pin with the onboard LED as an output.
    pinMode(LED_PIN, OUTPUT);
    
    // Initialise the hardware timer for counting seconds.
    wiggle_timer_init();

    // Initialise then enable the timer used for the LED pulse effect.
    led_pwm_timer_init();
    led_pulse(true);

    // Initialise the mouse capability using the Arduino Mouse library.
    Mouse.begin();

    // Globally enable interrupts.  Redundant? (triggered by Mouse.begin();.
    sei(); 
}

void loop(void) {

    if (global_seconds >= TIME_TO_WIGGLE) {
        led_pulse(false);           // Disable LED pulse effect.
        digitalWrite(LED_PIN, HIGH);// Turn on the LED (full brightness).
        global_seconds = 0;         // Reset the seconds counter.
        wiggle();                   // Move the mouse cursor.
        delay(LED_FLASH_MS);        // Pause with LED still at full-brightness.
        led_pulse(true);            // Resume the LED pulse effect.
    }

    delay(500);
}
