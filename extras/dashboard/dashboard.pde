// dashboard.pde - Processing sketch to visualize a Nunchuk sensor values

// Get Processing: https://processing.org/download 
// Run this while the ESP with Nunchuk, running the Arduino code of nunchuk_dump.ino, is connected to the PC.
// Make sure Arduino IDE does NOT have the serial port open.

// Note: the accelerometers measure earth's G-force, so Nunchuk rotations around the 
// Z-axis, that is the X/Y plane can not be measured.

import processing.serial.*;
Serial port;


// Drawing constants
final int   unit=30;
final int   sep= unit/2;
final int   font=20;


void setup() {
  size(400, 450, P3D);
  String portname = Serial.list()[1];
  port = new Serial(this, portname, 115200);
}


void draw() {
  int[] vals;
  if( port.available()==0 ) return;
  String str = port.readStringUntil('\n');
  try { vals = int(splitTokens(str,": ")); } catch(Exception e) { return; } 
  if( vals.length!=15 )  { return; }
  // Supposed to read "Z:1 C:0 joyX:-7 joyY:5 accelX:-182 accelY:-89 accelZ:102\n"
  // Indeces           0 1 2 3 4    5  6    7 8      9    10     11  12     13 14
  
  background(160, 160, 160);
  textSize(20);
  
  float butc= vals[3]; 
  int y=sep;
  fill(20,20,20); text("C", 200-2*font, y+font); 
  if( butc>0.5 ) fill(255,0,0); else fill(200,200,200);
  rect(200-unit/2,y, unit,unit);

  float butz= vals[1]; 
  y+=unit+sep;
  fill(20,20,20); text("Z", 200-2*font, y+font); 
  if( butz>0.5 ) fill(255,0,0); else fill(200,200,200);
  rect(200-unit/2,y, unit,unit); 
  
  float joyx= vals[5]; 
  y+=unit+sep;
  fill(20,20,20); text("joyx", 200-128, y-font+font/2); 
  fill(200,200,200);
  rect(200-128,y, 256, unit);
  fill(255,0,0); 
  if( vals[5]<0 )
    rect(200+joyx,y, -joyx,unit);
  else
    rect(200,y, joyx, unit);

  float joyy= vals[7]; 
  y+=unit+sep;
  fill(20,20,20); text("joyy", 200-unit/2-2*font, y+font); 
  fill(200,200,200);
  rect(200-unit/2,y, unit,256);
  fill(255,0,0); 
  if( joyy<0 )
    rect(200-unit/2,y+128, unit,-joyy);
  else
    rect(200-unit/2,y+128-joyy, unit,joyy);

  float accelx = vals[9];
  int x= 200+unit/2+sep+sep;
  fill(20,20,20); text("x", x, y+256+font); 
  fill(200,200,200);
  rect(x,y, unit/2,256);
  fill(0,255,0); 
  if( accelx<0 )
    rect(x,y+128, unit/2,-accelx/2);
  else
    rect(x,y+128-accelx/2, unit/2,accelx/2);
    
  float accely = vals[11];
  x+= unit/2+sep;
  fill(20,20,20); text("y", x, y+256+font); 
  fill(200,200,200);
  rect(x,y, unit/2,256);
  fill(0,255,0); 
  if( accely<0 )
    rect(x,y+128, unit/2,-accely/2);
  else
    rect(x,y+128-accely/2, unit/2,accely/2);
  
  float accelz = vals[13];
  x+= unit/2+sep;
  fill(20,20,20); text("z", x, y+256+font); 
  fill(200,200,200);
  rect(x,y, unit/2,256);
  fill(0,255,0); 
  if( accelz<0 )
    rect(x,y+128, unit/2,-accelz/2);
  else
    rect(x,y+128-accelz/2, unit/2,accelz/2);
}
