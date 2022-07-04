# wiggl3r

Use a simple [ATmega32U4][link_web_atmega32u4] microcontroller dev-board to workaround frequent PC timeouts/auto-locking.

This device will pretend to be a generic mouse when plugged into a USB port.  Every minute it will move the mouse cursor a single pixel so the PC thinks someone is still there.

## Hardware

This was achieved with a cheap ATmega32U4 dev-board with card-edge contacts for directly plugging into a USB type-A port.

The dev-board is a "beetle" and is firmware-compatible with the [Arduino Leonardo][link_web_arduino_leonardo].

There is an on-board LED that provides a pulsing heart-beat, and briefly switches to full-brightness when it moves the cursor.

## Firmware

Thanks to the [Arduino][link_web_arduino] framework, the firmware was slapped together very quickly.  However, the Arduino IDE was not used.  Instead the firmware was developed in [VSCode][link_web_vscode] using the [PlatformIO][link_web_platformio] extension.

A few Arduino functions were used, as well as an Arduino library:
1. Arduino digital I/O funcions - [pinMode()][link_web_arduino_pinmode], [digitalWrite()][link_web_arduino_digitalwrite].  Used for setting the LED as an output and swithcing it on/off.
2. Arduino analog I/O funcion - [analogWrite()][link_web_arduino_analogwrite].  Used to control a PWM signal for varying the LED brightness. 
3. Arduino [Mouse library][link_web_arduino_mouse] - Mouse.begin() and Mouse.move() functions.  Used to emulate a USB mouse.

Arduino doesn't seem to maintain any timer/counter interrupt libraries/functions.  Third-party options are available, but instead the timers used were configured by setting the corresponding registers directly and writing specific functions and interrupt sub-routines.
1. Timer/Counter 1 - configured to trigger an interrupt every second.  This interrupt was used to track seconds elapsed since last moving the mouse cursor.
2. Timer/Counter 3 - configured to update the LED brightness to create the pulsing effect.

---

![Photo of the dev board.][image_wiggl3r_photo]

---

![Animation showing the LED pulse.][image_wiggl3r_gif]

[image_wiggl3r_photo]:/data/wiggl3r.jpg
[image_wiggl3r_gif]:/data/wiggl3r.gif

[link_web_arduino]:https://www.arduino.cc/
[link_web_arduino_analogwrite]:https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/
[link_web_arduino_digitalwrite]:https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/
[link_web_arduino_leonardo]:https://docs.arduino.cc/hardware/leonardo
[link_web_arduino_mouse]:https://www.arduino.cc/reference/en/language/functions/usb/mouse/
[link_web_arduino_pinmode]:https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
[link_web_atmega32u4]:https://www.microchip.com/en-us/product/ATmega32U4
[link_web_platformio]:https://platformio.org/
[link_web_vscode]:https://code.visualstudio.com/
