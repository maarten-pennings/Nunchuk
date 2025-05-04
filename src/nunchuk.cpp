// nunchuk.cpp - an Arduino library to control a (real or cloned) Nunchuk.
#include "nunchuk.h"   // own


// The Nunchuk I2C address
#define NUNCHUK_ADDRESS         0x52

// Nunchuk registers
#define NUNCHUK_REG_DATA        0x00
#define NUNCHUK_REG_ID          0xFA


// Private data
static TwoWire * nunchuk_wire;    // handle to i2C driver
static uint8_t   nunchuk_data[6]; // last read data by nunchuk_scan()


// Returns the Nunchuk id
uint32_t nunchuk_id() {
  uint32_t id=0;
  nunchuk_wire->beginTransmission(NUNCHUK_ADDRESS);
  nunchuk_wire->write(NUNCHUK_REG_ID);
  nunchuk_wire->endTransmission(true); // Repeated start does NOT work with real Nunchuk
  nunchuk_wire->requestFrom(NUNCHUK_ADDRESS, 6, 1/*STOP*/);
  if( nunchuk_wire->available() != 6 ) return false;
  if( nunchuk_wire->read()!=0 ) return 0; // Error, first byte must be 0x00
  if( nunchuk_wire->read()!=0 ) return 0; // Error, second byte must be 0x00
  for(int i=2; i<6; i++ ) id= id*256 + nunchuk_wire->read(); // next 4 bytes is ID
  return id;
}


// Initialize the Nunchuk - returns true if found
int nunchuk_begin(TwoWire *wire) {
  if( wire==0 ) { // Nunchuk driver begin also begins I2C if non is passed
    Wire.begin(); // default pins
    Wire.setClock(100000); // real Nunchuk fails on 400kHz
    nunchuk_wire= &Wire;
  } else { // Caller begins wire (picking specific pins and freq)
    nunchuk_wire= wire;
  }

  nunchuk_wire->beginTransmission(NUNCHUK_ADDRESS);
  nunchuk_wire->write(0xF0);
  nunchuk_wire->write(0x55);
  nunchuk_wire->endTransmission(1/*STOP*/);

  nunchuk_wire->beginTransmission(NUNCHUK_ADDRESS);
  nunchuk_wire->write(0xFB);
  nunchuk_wire->write(0x00);
  nunchuk_wire->endTransmission(1/*STOP*/);

  delayMicroseconds(200); // real Nunchuk requires delay here

  return nunchuk_id()==NUNCHUK_ID;
}


// Read the raw sensor values from the Nunchuk into the Arduino memory. Returns true if successful.
int nunchuk_scan() {
  nunchuk_wire->beginTransmission(NUNCHUK_ADDRESS);
  nunchuk_wire->write(NUNCHUK_REG_DATA);
  nunchuk_wire->endTransmission(1/*STOP*/); // Repeated start does NOT work with real Nunchuk
  delayMicroseconds(200); // The real Nunchuk requires delay here
  nunchuk_wire->requestFrom(NUNCHUK_ADDRESS, 6, 1/*STOP*/);
  if( nunchuk_wire->available()!=6 ) return false;
  nunchuk_wire->readBytes(nunchuk_data,6);
  return true;
}


// Returns the state of button Z - from latest nunchuk_scan().
int nunchuk_buttonZ() {
  int raw = (nunchuk_data[5]>>0) & 0x01;
  return raw == 0;
}


// Returns the state of button C - from latest nunchuk_scan().
int nunchuk_buttonC() {
  int raw = (nunchuk_data[5]>>1) & 0x01;
  return raw == 0;
}


// Returns the X-value of the joystick - from latest nunchuk_scan().
int nunchuk_joystickX() {
  int raw = nunchuk_data[0];
  return raw - 128; // The real Nunchuk has a smaller range then -128..+127
}


// Returns the Y-value of the joystick - from latest nunchuk_scan().
int nunchuk_joystickY() {
  int raw = nunchuk_data[1];
  return raw - 128; // The real Nunchuk has a smaller range then -128..+127
}


// Returns the X-value of the accelerometer - from latest nunchuk_scan().
int nunchuk_accelX() {
  int raw = (nunchuk_data[2]<<2) | ((nunchuk_data[5]>>2) & 0x03);
  return raw - 512;
}


// Returns the Y-value of the accelerometer - from latest nunchuk_scan().
int nunchuk_accelY() {
  int raw = (nunchuk_data[3]<<2) | ((nunchuk_data[5]>>4) & 0x03);
  return raw - 512;
}


// Returns the Z-value of the accelerometer - from latest nunchuk_scan().
int nunchuk_accelZ() {
  int raw = (nunchuk_data[4]<<2) | ((nunchuk_data[5]>>6) & 0x03);
  return raw - 512;
}


// Low pass filter implementation
#define LPF_MAXFACTOR           1024 
#define DIV_FACTOR(v)           ( ( (v) + LPF_MAXFACTOR/2 ) / LPF_MAXFACTOR )
#define LPF_UPDATE(var,val,fac) ( (var) = DIV_FACTOR( (var)*(fac) + (val)*(LPF_MAXFACTOR-(fac)) ) )


// Low pass filter accumulators
static uint16_t  nunchuk_x;       // accumulator for low-pass filter for x accelerometer
static uint16_t  nunchuk_y;       // accumulator for low-pass filter for y accelerometer
static uint16_t  nunchuk_z;       // accumulator for low-pass filter for z accelerometer


// Call once after each nunchuk_scan() to update LPF accumulators.
// factor = 0..1024, 0 is no filtering (always raw), 1024 is max filtering (raw ignored)
void nunchuk_lpf(int factor) {
  int raw;
  raw = (nunchuk_data[2]<<2) | ((nunchuk_data[5]>>2) & 0x03);
  LPF_UPDATE(nunchuk_x,raw,factor);
  raw = (nunchuk_data[3]<<2) | ((nunchuk_data[5]>>4) & 0x03);
  LPF_UPDATE(nunchuk_y,raw,factor);
  raw = (nunchuk_data[4]<<2) | ((nunchuk_data[5]>>6) & 0x03);
  LPF_UPDATE(nunchuk_z,raw,factor);
}


// Returns the low-pass-filtered X-value of the accelerometer - from latest nunchuk_scan()/nunchuk_lpf() pair.
int nunchuk_accelXlpf() {
  return nunchuk_x - 512;
}


// Returns the low-pass-filtered Y-value of the accelerometer - from latest nunchuk_scan()/nunchuk_lpf() pair.
int nunchuk_accelYlpf() {
  return nunchuk_y - 512;
}


// Returns the low-pass-filtered Z-value of the accelerometer - from latest nunchuk_scan()/nunchuk_lpf() pair.
int nunchuk_accelZlpf() {
  return nunchuk_z - 512;
}

