// nunchuk64.ino - using a nunchuk on a commodore 64 (C64) via ESP32-C3 Super Mini
// Compile in Arduino with board "Nologo ESP32C3 Super Mini"
// 20250802 Maarten Pennings - v2 Robust for Nunchuk replug
// 20250508 Maarten Pennings - v1 Created


// OUTPUT
// Welcome at nunchuk64.ino v2
// 20250802 Maarten Pennings
// led: blinking=booting; on=no-contact; flash=action
// 
// booting success
// nunchuk init success
// 1 F0 N1 E0 S0 W0
// 2 F0 N1 E0 S0 W0
// 3 F0 N0 E1 S0 W0
// 4 F0 N0 E1 S0 W0
// 5 F0 N0 E1 S0 W0
// 6 F0 N0 E1 S0 W0
// 7 F0 N1 E1 S0 W0
// 8 F0 N1 E1 S0 W0
// 9 F0 N1 E1 S0 W0
// 10 F1 N0 E0 S0 W0
// 11 F1 N0 E0 S0 W0


#include <nunchuk.h>


// Pin (and values) for signalling LED
const int pin_led   = 8;
const int led_off   = 1; // LED is low active
const int led_on    = 0;

// Pins for I2C bus
const int pin_sda   = 7; // ESP32-C3 super mini: skipping SDA on pin 8 (it also has LED)
const int pin_scl   = 9;

// Pins (and values) for  joystick outputs (order as C64 port)
const int pin_fire  = 0;
const int pin_north = 1;
const int pin_south = 2;
const int pin_west  = 3;
const int pin_east  = 4;
const int joy_off   = 0; // Via open drain n-channel mosfet (BS138)
const int joy_on    = 1;
const int joy_min   =40;


// System state
int count;                 // Number of actions
int mode;                  // See MODE_XXX
int errors;                // Counts number of failed scan() attempts (move to fatal if too many) while RUNNING

#define MODE_INIT       1  // If init is needed: after power up and after too many errors 
#define MODE_RUNNING    2  // Normal operational mode: checking Nunchuk and manipulating joystick pins
#define ERRORTHRESHOLD 10  // if `errors` exceeds this threshold, `needsinit` is reaised.


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
  Serial.printf("\n\n\n");
  Serial.printf("Welcome at nunchuk64.ino v2\n");
  Serial.printf("20250802 Maarten Pennings\n");
  Serial.printf("led: blinking=booting; on=no-contact; flash=action\n");
  Serial.printf("\n");

  // Init ESP32: joystick pins
  pinMode(pin_fire , OUTPUT);
  pinMode(pin_north, OUTPUT);
  pinMode(pin_east , OUTPUT);
  pinMode(pin_south, OUTPUT);
  pinMode(pin_west , OUTPUT);

  // Init ESP32: no joystick output active on startup
  digitalWrite(pin_fire , joy_off);
  digitalWrite(pin_north, joy_off);
  digitalWrite(pin_east , joy_off);
  digitalWrite(pin_south, joy_off);
  digitalWrite(pin_west , joy_off);

  // Init system state
  count=0;
  mode=MODE_INIT;
  Serial.printf("booting success\n");
}


void loop() {

  if( mode==MODE_INIT ) {
    // Connect to nunchuck
    Wire.begin(pin_sda, pin_scl, 100*1000); 
    if( nunchuk_begin(&Wire) ) { 
      Serial.printf("nunchuk init success\n"); 
      digitalWrite(pin_led, led_off);
      mode=MODE_RUNNING;
      errors=0; // only used while RUNNING
    } else {
      Serial.printf("nunchuk init failed\n"); 
      digitalWrite(pin_led, led_on);
      mode=MODE_INIT; // stay in INIT
    }
  }

  if( mode==MODE_RUNNING ) {
    // Scan nunchuk state
    if( nunchuk_scan() ) {
      errors=0; // scan() successful; reset error counter

      // No need for accelerometer, so no low-pass filter

      // Determine state for the 5 joystick outputs
      int fire  = nunchuk_buttonZ() || nunchuk_buttonC();
      int north = nunchuk_joystickY() > +joy_min ;
      int east  = nunchuk_joystickX() > +joy_min ;
      int south = nunchuk_joystickY() < -joy_min ;
      int west  = nunchuk_joystickX() < -joy_min ;

      // Is there any activity, print and show on LED
      int action = fire || north || east || south || west;
      if( action ) Serial.printf("%d F%d N%d E%d S%d W%d\n", ++count,fire,north,east,south,west);
      digitalWrite(pin_led  , action ? led_on : led_off );

      // Copy state to pin
      digitalWrite(pin_fire , fire   ? joy_on : joy_off );
      digitalWrite(pin_north, north  ? joy_on : joy_off );
      digitalWrite(pin_east , east   ? joy_on : joy_off );
      digitalWrite(pin_south, south  ? joy_on : joy_off );
      digitalWrite(pin_west , west   ? joy_on : joy_off );

    } else {
      errors++;
      if( errors>ERRORTHRESHOLD ) { 
        Serial.printf("nunchuk scan failed\n"); 
        mode=MODE_INIT;
      }
    }
  }

  delay(50);
} 
