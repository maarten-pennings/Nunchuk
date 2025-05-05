// nunchuk_dump.ino - demo outputing live sensor data from a connected Nunchuk


#include <nunchuk.h>


// Run this example with the Nunchuk connected as explained in the readme.md.
// View the results either
// - Open Tools > Serial Monitor (the numbers)
// - Open Tools > Serial Plotter (the numbers graphed)
// - Start extras/dashboard/dashboard.pde (after installing https://processing.org/)
// - Start extras/nunchuk_viewer/nunchuk_viewer.pde (after installing https://processing.org/)


void setup() {
  Serial.begin(115200);
  Serial.print("\n\nNunchuk_dump.ino\n");

  if( !nunchuk_begin() ) { Serial.println("Nunchuk begin failed"); delay(2000); }
}


void loop() {
  if( nunchuk_scan() ) {
    nunchuk_lpf(); // low pass filter for accel's
    Serial.printf("Z:%d C:%d joyX:%d joyY:%d accelX:%d accelY:%d accelZ:%d \n", 
      nunchuk_buttonZ(), nunchuk_buttonC(), 
      nunchuk_joystickX(), nunchuk_joystickY(), 
      nunchuk_accelXlpf(), nunchuk_accelYlpf(), nunchuk_accelZlpf() 
    );
  }  
  delay(25);
} 
