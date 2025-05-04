// nunchuk.h - an Arduino library to control a (real or cloned) Nunchuk.
#ifndef _NUNCHUK_H_
#define _NUNCHUK_H_


#include <stdint.h>  // uint16_t
#include <Wire.h>    // TwoWire


// The Nunchuk ID - hopefully returned by nunchuk_id()
#define NUNCHUK_ID              0xA4200000




int  nunchuk_begin(TwoWire *wire=0);// Initializes the Nunchuk (and wire if 0 passed) - returns true if Nunchuk found
uint32_t nunchuk_id();              // Returns the Nunchuk id

int  nunchuk_scan();                 // Read the raw sensor values from the Nunchuk into the Arduino memory. Returns true if successful.
     
int  nunchuk_buttonZ();              // Returns the state of Z button - from latest nunchuk_scan().
int  nunchuk_buttonC();              // Returns the state of C button - from latest nunchuk_scan().
int  nunchuk_joystickX();            // Returns the X-value of the joystick (-128..0..+127) - from latest nunchuk_scan().
int  nunchuk_joystickY();            // Returns the Y-value of the joystick (-128..0..+127)  - from latest nunchuk_scan().
int  nunchuk_accelX();               // Returns the X-value of the accelerometer (-512..0..+511) - from latest nunchuk_scan().
int  nunchuk_accelY();               // Returns the Y-value of the accelerometer (-512..0..+511) - from latest nunchuk_scan().
int  nunchuk_accelZ();               // Returns the Z-value of the accelerometer (-512..0..+511) - from latest nunchuk_scan().


void nunchuk_lpf(int factor=750);    // Call once after each nunchuk_scan() to update LPF accumulators. factor= 0..1024; 0 is no filtering (always raw, fast but jumpy)
int  nunchuk_accelXlpf();            // Returns the low-pass-filtered X-value of the accelerometer - from latest nunchuk_scan()/nunchuk_lpf() pair.
int  nunchuk_accelYlpf();            // Returns the low-pass-filtered Y-value of the accelerometer - from latest nunchuk_scan()/nunchuk_lpf() pair.
int  nunchuk_accelZlpf();            // Returns the low-pass-filtered Z-value of the accelerometer - from latest nunchuk_scan()/nunchuk_lpf() pair.


#endif
