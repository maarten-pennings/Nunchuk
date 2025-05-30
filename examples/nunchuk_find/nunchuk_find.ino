// nunchuk_find.ino - scans i2C bus to find the Nunchuk
#include <Wire.h>

// This is the output on my machine with a real Nunchuk connected.
// Note sure about the addresses<8 or >77; but 0x52 is the expected one.
//
// nunchuk_find I2C scanner
//
//  00 [01][02][03][04][05][06][07] 08  09  0A  0B  0C  0D  0E  0F 
//  10  11  12  13  14  15  16  17  18  19  1A  1B  1C  1D  1E  1F 
//  20  21  22  23  24  25  26  27  28  29  2A  2B  2C  2D  2E  2F 
//  30  31  32  33  34  35  36  37  38  39  3A  3B  3C  3D  3E  3F 
//  40  41  42  43  44  45  46  47  48  49  4A  4B  4C  4D  4E  4F 
//  50  51 [52] 53  54  55  56  57  58  59  5A  5B  5C  5D  5E  5F 
//  60  61  62  63  64  65  66  67  68  69  6A  6B  6C  6D  6E  6F 
//  70  71  72  73  74  75  76  77 [78][79][7A][7B][7C][7D][7E][7F]


int i=0;
void setup() {
  Serial.begin(115200);
  Serial.printf("\n\nnunchuk_find I2C scanner\n\n");
  Wire.begin();
}


void loop() {
  for( int address=0x00; address<0x80; address++ ) {
    Wire.beginTransmission(address);
    int error = Wire.endTransmission();

    if( error==0 ) Serial.printf("["); else Serial.printf(" ");
    Serial.printf("%02X", address);
    if( error==0 ) Serial.printf("]"); else Serial.printf(" ");

    if( address%16==15 ) Serial.printf("\n");
  }
  Serial.printf("\n");
  delay(5000); // wait 5 seconds for next scan
}
