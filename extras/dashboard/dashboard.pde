// dashboard.pde - Processing sketch to visualize Nunchuk sensor values

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
  // Indices           0 1 2 3 4    5  6    7 8      9    10     11  12     13 14
  // Get button data
  int butZ   = vals[1]; 
  int butC   = vals[3]; 
  int joyX   = vals[5]; 
  int joyY   = vals[7]; 
  int accelX = vals[9];
  int accelY = vals[11];
  int accelZ = vals[13];

  // Start drawing  
  background(160, 160, 160);
  textSize(font);
  
  fill(20,20,20); 
  text("Nunchuk", 20, 450-2*font); 
  text("Dashboard", 20, 450-1*font); 
  
  int y=sep;
  fill(20,20,20); text("C", 200-2*font, y+font); 
  if( butC==1 ) fill(255,0,0); else fill(200,200,200);
  rect(200-unit/2,y, unit,unit);

  y+=unit+sep;
  fill(20,20,20); text("Z", 200-2*font, y+font); 
  if( butZ==1 ) fill(255,0,0); else fill(200,200,200);
  rect(200-unit/2,y, unit,unit); 
  
  y+=unit+sep;
  fill(20,20,20); text("joyx", 200-128, y-font+font/2); 
  fill(200,200,200);
  rect(200-128,y, 256, unit);
  fill(255,0,0); 
  if( joyX<0 )
    rect(200+joyX,y, -joyX,unit);
  else
    rect(200,y, joyX, unit);

  y+=unit+sep;
  fill(20,20,20); text("joyy", 200-unit/2-2*font, y+font); 
  fill(200,200,200);
  rect(200-unit/2,y, unit,256);
  fill(255,0,0); 
  if( joyY<0 )
    rect(200-unit/2,y+128, unit,-joyY);
  else
    rect(200-unit/2,y+128-joyY, unit,joyY);

  int x= 200+unit/2+sep+sep;
  fill(20,20,20); text("x", x, y+256+font); 
  fill(200,200,200);
  rect(x,y, unit/2,256);
  fill(0,255,0); 
  if( accelX<0 )
    rect(x,y+128, unit/2,-accelX/2);
  else
    rect(x,y+128-accelX/2, unit/2,accelX/2);
    
  x+= unit/2+sep;
  fill(20,20,20); text("y", x, y+256+font); 
  fill(200,200,200);
  rect(x,y, unit/2,256);
  fill(0,255,0); 
  if( accelY<0 )
    rect(x,y+128, unit/2,-accelY/2);
  else
    rect(x,y+128-accelY/2, unit/2,accelY/2);
  
  x+= unit/2+sep;
  fill(20,20,20); text("z", x, y+256+font); 
  fill(200,200,200);
  rect(x,y, unit/2,256);
  fill(0,255,0); 
  if( accelZ<0 )
    rect(x,y+128, unit/2,-accelZ/2);
  else
    rect(x,y+128-accelZ/2, unit/2,accelZ/2);
}
