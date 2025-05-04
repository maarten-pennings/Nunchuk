# Nunchuk

An Arduino library to control a Nunchuk.
It seems Nunchuk is the official spelling, not Nunchu**c**k.


## Introduction

I found out a Nunchuk is actually an I2C device.
It has I2C address 0x52 and accepts the fast protocol (400kHz).
It runs on 3.3V - although rumors has it that it works on 5V too.

The device needs hardly any writing, mostly just reading the sensors.
The Nunchuk has the following sensors:
 - The Z-button (1 bit)
 - The C-button (1 bit)
 - A potentiometer for the horizontal axis in the joystick (8 bit) 
 - A potentiometer for the vertical axis in the joystick (8 bit) 
 - An accelerometer in the X, Y, and Z direction (10 bit each)


Information from
 - [bootlin](https://bootlin.com/labs/doc/nunchuk.pdf)
 - [wiibre](https://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck)


## I2C device

The pinout of the Nunchuk is not hard, but most drawings you find on the internet
don't tell whether you are looking at the plug of the Nunchuk or the outlet of the Wii controller.
To avoid all confusion, here a photo of the Nunchuk plug, with the meaning of the pins.
Also note the notch on the bottom side, ensuring the plug goes in one-way only.

![Nunchuk pinout](Nunchuk-pinout.jpg)

The center pin at the bottom is not connected in my (Ali Express Nunchuk).
The center pin at the top is connected, rumor goes it is a device-detect pin.



```
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+
  | Byte  | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+
  |   0   |                        joystick x [7:0]                       |
  |   1   |                        joystick y [7:0]                       |
  |   2   |                     accelerometer x [9:2]                     |
  |   3   |                     accelerometer y [9:2]                     |
  |   4   |                     accelerometer z [9:2]                     |
  |   5   | accel z [1:0] | accel y [1:0] | accel x [1:0] | but C | but Z |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+
  | Byte  | bit 7 | bit 6 | bit 5 | bit 4 | bit 3 | bit 2 | bit 1 | bit 0 |
  +-------+-------+-------+-------+-------+-------+-------+-------+-------+
```


## Images

![Wiring](wiring.jpg)

![X-ray from dangerousprototypes](http://dangerousprototypes.com/docs/images/1/14/Wii-nunchuck.jpg)

(end)
  
  
  
