// nunchuk64.ino - using a nunchuk on a commodore 64 (C64) via ESP32-C3 Super Mini
// Compile in Arduino with board "Nologo ESP32C3 Super Mini"
// 20250508 Maarten Pennings


// OUTPUT
// upon startup: quick flashes, then Welcome over Serial
// upon action (here the fire button is pressed): state over Serial, also LED switches on
//
// Welcome at nunchuk64.ino
// 20250508 Maarten Pennings
//
// 1 F1 N0 E0 S0 W0
// 2 F1 N0 E0 S0 W0
// 3 F1 N0 E0 S0 W0
// 4 F1 N0 E0 S0 W0
// 5 F1 N0 E0 S0 W0
// 6 F1 N0 E0 S0 W0


#include <nunchuk.h>


// Pin (and values) for signalling LED
const int pin_led   = 8;
const int led_off   = 1; // LED is low active
const int led_on    = 0;

// Pins for I2C bus
const int pin_sda   = 7;
const int pin_scl   = 9;

// Pins (and values) for  joystick outputs
const int pin_fire  = 0;
const int pin_north = 1;
const int pin_east  = 2;
const int pin_south = 3;
const int pin_west  = 4;
const int joy_off   = 0; // Via open drain n-channel mosfet (BS138)
const int joy_on    = 1;


// System state
int count;  // Number of actions
int fatal;  // Set if there was a fatal error
int errors; // Counts number of failed scan() attempts


void setup() {
  // Blink as power-on feedback, and to allow USB to connect
  pinMode(pin_led, OUTPUT);
  for( int i=0; i<15; i++ ) {
    digitalWrite(pin_led, led_on);  delay(50);
    digitalWrite(pin_led, led_off); delay(50);
    // Terminate with LED off
  }

  // Print welcome message
  Serial.begin(115200);
  Serial.print("\n\n\n");
  Serial.print("Welcome at nunchuk64.ino\n");
  Serial.print("20250508 Maarten Pennings\n\n");

  // Init error flags
  count=0;
  fatal=0;
  errors=0;

  // Init joystick pins
  pinMode(pin_fire , OUTPUT);
  pinMode(pin_north, OUTPUT);
  pinMode(pin_east , OUTPUT);
  pinMode(pin_south, OUTPUT);
  pinMode(pin_west , OUTPUT);

  // No joystick output active on startup
  digitalWrite(pin_fire , joy_off);
  digitalWrite(pin_north, joy_off);
  digitalWrite(pin_east , joy_off);
  digitalWrite(pin_south, joy_off);
  digitalWrite(pin_west , joy_off);

  // Connect to nunchuck
  Wire.begin(pin_sda, pin_scl, 100*1000); // ESP32-C3 super mini, skipping SDA on pin 8 (it also has LED)
  if( ! nunchuk_begin(&Wire) ) { 
    fatal= 1;
    Serial.println("begin() failed"); 
    digitalWrite(pin_led, led_on);
  }
}


void loop() {
  // Bail out if fatal
  if( fatal ) return;

  // Read nunchuk (if too many fails, go fatal)
  if( ! nunchuk_scan() ) {
    errors++;
    if( errors>50 ) { 
      fatal= true;
      Serial.println("scan() failed"); 
      digitalWrite(pin_led, led_on);
    }
    return;
  }
  errors=0; // scan() successful; reset counter

  // No need fo accelerometer, so no lpf filter

  // Determine state for the 5 joystick outputs
  int fire  = nunchuk_buttonZ() || nunchuk_buttonC();
  int north = nunchuk_joystickY() > +20 ;
  int east  = nunchuk_joystickX() > +20 ;
  int south = nunchuk_joystickY() < -20 ;
  int west  = nunchuk_joystickX() < -20 ;

  // Is there any activity, print and show on LED
  int action = fire || north || east || south || west;
  if( action ) Serial.printf("%d F%d N%d E%d S%d W%d\n", ++count,fire,north,east,south,west);
  digitalWrite(pin_led  , action?led_on:led_off);

  // Copy state to pin
  digitalWrite(pin_fire , fire  ?joy_on:joy_off);
  digitalWrite(pin_north, north ?joy_on:joy_off);
  digitalWrite(pin_east , east  ?joy_on:joy_off);
  digitalWrite(pin_south, south ?joy_on:joy_off);
  digitalWrite(pin_west , west  ?joy_on:joy_off);

  delay(25);
} 
