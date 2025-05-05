// nunchuk.h - an Arduino library to control a (real or cloned) Nunchuk.
#ifndef _NUNCHUK_H_
#define _NUNCHUK_H_


#include <stdint.h>  // uint16_t
#include <Wire.h>    // TwoWire


// The Nunchuk ID - hopefully returned by `nunchuk_id()`.
#define NUNCHUK_ID              0xA4200000


// Initializes the Nunchuk (and I2C if `wire==0`) - returns `true` if Nunchuk found.
int  nunchuk_begin(TwoWire *wire=0);
// Returns the Nunchuk id.
uint32_t nunchuk_id();

// Read the raw sensor values from the Nunchuk into the Arduino memory. Returns `true` if successful.
int  nunchuk_scan();

// Returns the state of Z button - from latest `nunchuk_scan()`.
int  nunchuk_buttonZ();
// Returns the state of C button - from latest `nunchuk_scan()`.
int  nunchuk_buttonC();
// Returns the X-value of the joystick (-128..0..+127) - from latest `nunchuk_scan()`.
int  nunchuk_joystickX();
// Returns the Y-value of the joystick (-128..0..+127)  - from latest `nunchuk_scan()`.
int  nunchuk_joystickY();
// Returns the X-value of the accelerometer (-512..0..+511) - from latest `nunchuk_scan()`.
int  nunchuk_accelX();
// Returns the Y-value of the accelerometer (-512..0..+511) - from latest `nunchuk_scan()`.
int  nunchuk_accelY();
// Returns the Z-value of the accelerometer (-512..0..+511) - from latest `nunchuk_scan()`.
int  nunchuk_accelZ();


// This driver includes a low-pass-filter (LPF) for the accelerometers.
// Call this function once, after each `nunchuk_scan()` call, to update LPF accumulators.
// Then use `nunchuk_accel[XYZ]lpf` instead of `nunchuk_accel[XYZ]`.
// Parameter `factor`= 0..1024; 0 is no filtering (always raw, fast but jumpy).
void nunchuk_lpf(int factor=750);
// Returns the low-pass-filtered X-value of the accelerometer (-512..0..+511) - from latest nunchuk_scan()/nunchuk_lpf() pair.
int  nunchuk_accelXlpf();
// Returns the low-pass-filtered Y-value of the accelerometer (-512..0..+511) - from latest nunchuk_scan()/nunchuk_lpf() pair.
int  nunchuk_accelYlpf();
// Returns the low-pass-filtered Z-value of the accelerometer (-512..0..+511) - from latest nunchuk_scan()/nunchuk_lpf() pair.
int  nunchuk_accelZlpf();


#endif
