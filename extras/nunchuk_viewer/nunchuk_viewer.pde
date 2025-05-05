// nunchuk_viewer.pde - Processing sketch to visualize Nunchuk sensor values by drawing the Nunchuk in 3D

// Get Processing: https://processing.org/download 
// Run this while the ESP with Nunchuk, running the Arduino code of nunchuk_dump.ino, is connected to the PC.
// Make sure Arduino IDE does NOT have the serial port open.

import processing.serial.*;
Serial port;
final int font=20;

void setup() {
  size(450, 450, P3D);
  
  String portname = Serial.list()[1];
  port = new Serial(this, portname, 115200);
}


void draw() {
  // Get Nunchuk sensor data from COM port
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
  
  // Compute angles
  float YZ = atan2(accelY, accelZ) - 0.1; // the offset needs to be callibrated
  float XZ = atan2(accelX, accelZ) + 0.1; // the offset needs to be callibrated

  // Start drawing
  background(100, 100, 100);
  lights();
  fill(250, 250, 250);
  textSize(font);
  text("Nunchuk", 20, 20); 
  text("Viewer", 20, 20+font); 
  
  pushMatrix();
    translate(225, 150, 0);
    rotateX(YZ-PI/2);
    rotateY(-XZ);
    drawNunchuk(butZ,butC,joyX,joyY);
  popMatrix();
}

void drawNunchuk(int butZ, int butC, int joyX, int joyY) {
  float radius = 30; // of controls
  float extend1 = 30; // how much top of controls is extended wrt to bottom
  float extend2 = 60;
  float objheight = 60; // height of controls
  float objdepth = -170; // length of hand
  float tighten = 0.5;
  int num= 20;
  
  // Compute vertex coordinates for rounding of "controls part"
  float[] vx = new float[num];
  float[] vy = new float[num];
  float sector = PI/(num-1);
  for( int i=0; i<num; i++) {
    vx[i] = cos( i*sector ) * radius;
    vy[i] = sin( i*sector ) * radius;
  }

  // "Controls part" top
  beginShape();
    vertex(radius,-extend1,0);
    for( int i=0; i<num; i++) vertex(vx[i], vy[i], 0);
    vertex(-radius,-extend1,0);
  endShape(CLOSE);
  // "Controls part" bottom 
  beginShape(); 
    for( int i=0; i<num; i++) vertex(vx[i], vy[i], objheight); 
  endShape(CLOSE);
  // Connect "Controls part" top to bottom
  beginShape(QUAD_STRIP); 
    vertex(radius,-extend1,0);
    vertex(radius,0,objheight);
    for( int i=0; i<num; i++) { vertex(vx[i], vy[i], 0); vertex(vx[i], vy[i], objheight); } 
    vertex(-radius,-extend1,0);
    vertex(-radius,0,objheight);
  endShape(CLOSE);

  // "Handle part" top
  beginShape();
    vertex(+radius*tighten,-extend1-extend2,-objdepth);
    vertex(+radius,-extend1,0);
    vertex(-radius,-extend1,0);
    vertex(-radius*tighten,-extend1-extend2,-objdepth);
  endShape(CLOSE);
  // "Handle part" bottom
  beginShape();
    vertex(+radius*tighten,-extend2,-objdepth);
    vertex(+radius,0,objheight);
    vertex(-radius,0,objheight);
    vertex(-radius*tighten,-extend2,-objdepth);
  endShape(CLOSE);
  // Connect "Handle part" top and bottom
  beginShape(QUAD_STRIP); 
    vertex(+radius,-extend1,0);
    vertex(+radius,0,objheight);
    vertex(+radius*tighten,-extend1-extend2,-objdepth);
    vertex(+radius*tighten,-extend2,-objdepth);
    vertex(-radius*tighten,-extend1-extend2,-objdepth);
    vertex(-radius*tighten,-extend2,-objdepth);
    vertex(-radius,-extend1,0);
    vertex(-radius,0,objheight);
  endShape(CLOSE);
  
  // Z button
  push();
    if( butZ==1 ) fill(250, 20, 20);
    translate(0, radius+(butZ==0?+0.3:-0.3)*objheight/3, 3*objheight/4); 
    box(objheight/3);
  pop();

  // C button
  push();
    if( butC==1 ) fill(250, 20, 20);
    translate(0, radius+(butC==0?+0.3:-0.3)*objheight/3, 1*objheight/4); 
    box(objheight/3);
  pop();

  // Joy
  push();
    if( abs(joyX)>20 || abs(joyY)>20 ) fill(250, 20, 20);
    float x= map(joyX, -128, +127, -radius, +radius );
    float y= map(joyY, -128, +127, -radius, +radius );
    translate(0.9*x, 0.9*y, 0); 
    box(radius/3);
  pop();

}
