/*
  DMX_Master.ino - Example code for using the Conceptinetics DMX library
  Copyright (c) 2013 W.A. van der Meeren <danny@illogic.nl>.  All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <Conceptinetics.h>
#include <math.h>

//
// CTC-DRA-10-R2 ISOLATED DMX-RDM SHIELD JUMPER INSTRUCTIONS
//
// Set !EN (right hand side) when uploading
// Set EN (left hand side) when running
// Or no jummper altogether and you can do all the things (in master mode)*
//   and you will see the lights blink to the data being sent over the line!
//
// * This will only work if the RX-???? jummper is also unset.


//
// The number of channels the master will control
//
#define DMX_MASTER_CHANNELS   170
const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0

// The above depends on amount of memory you have avaliable
// Control data will always be sent to all 255 channels
// A default of the first 100 channels will fit on a typical UNO
// A minimum of 1 channel
//
// If you are controlling lights for example:
// Channel 1 is white
// Channel 2 is Red
// Channel 3 is Green
// Channel 4 is Blue
//
// A total of 4 channels!

//
// Pin number to change read or write mode on the shield
//
#define RXEN_PIN                2
#define lamps 17

//
// A little macro that takes a percentage and chnages it to 0-255
//
#define MAX_VALUE 255
#define PER_TO_VAL(percent) ((MAX_VALUE/100) * percent)


typedef enum DmxCh {

  RED    = 0,
  GREEN  = 1,
  BLUE   = 2,
  WHITE  = 3

} DmxCh;


// Configure a DMX master controller, the master controller
// will use the RXEN_PIN to control its write operation
// on the bus
DMX_Master        dmx_master(DMX_MASTER_CHANNELS, RXEN_PIN);


// the setup routine runs once when you press reset:
void setup() {
  //Serial.begin(9600);

  // Enable DMX master interface and start transmitting
  dmx_master.enable();

  // Set channel 1 -THRU- 50 -ALL TO- 50%
  const int B = 4275;               // B value of the thermistor
  const int R0 = 100000;            // R0 = 100k
  const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A0
  int startChannel = 1;
  int endChannel   = DMX_MASTER_CHANNELS;
  int byteValue    = PER_TO_VAL(0); // RGB all to 50%

  ///////////
  // setChannelRange ( begin_channel, end_channel, byte_value );
  ///////////
  dmx_master.setChannelRange ( startChannel, endChannel, byteValue );
}

// the loop routine runs over and over again forever:
void loop() {
  //set_rgb_value(1, 255, 0, 0,0);
  //set_rgb_value(2, 0, 255, 0,0);
  //set_rgb_value(3, 0, 0, 255,0);
  //set_rgb_value(4, 0, 0, 0,255);
  // RunningLights(0xff, 0xff, 0x00, 100);
//  iceTwinkle();
///  simpleWave(0.03,5,10);
  //dmx_master.setChannelValue(5, 255);

  //  if (analogRead(0) > 550) {
  //    //supacold();
  //    pwarm();
  //  } else if (analogRead(0) < 550 && analogRead(0) > 400) {
  //kindaCold();
 Fire(20,50,20);
  //  } else {
      //supacold();
  //  }
}


void set_rgb_value(int lampNumber, int red, int green, int blue, int white) {
  int channelNumber = lampNumber * 10;

  // Set each of the colors
  dmx_master.setChannelValue(channelNumber + RED,   red  );
  dmx_master.setChannelValue(channelNumber + GREEN, green);
  dmx_master.setChannelValue(channelNumber + BLUE,  blue );
  dmx_master.setChannelValue(channelNumber + WHITE,  white );

}

void supacold() {

  for (int i = 0; i < 255; i++) {
    for (int j = 0; j < lamps; j++) {
      set_rgb_value(j, i+random(50), 0, 255, random(200));
    }
    delay(10);
  }
  for (int i = 255; i > 0; i--) {
    for (int j = 0; j < lamps; j++) {
      set_rgb_value(j, i+random(50), 0, 255, random(200));
    }
    delay(10);
  }

}
void kindaCold() {
  for (int i = 0; i < 255; i++) {
    for (int j = 0; j < lamps; j++) {
      set_rgb_value(j, i, 0, 255, 0);
    }
    delay(10);
  }
  for (int i = 255; i > 0; i--) {
    for (int j = 0; j < lamps; j++) {
      set_rgb_value(j, i, 0, 255, 0);
    }
    delay(10);
  }
}
void pwarm() {
  dmx_master.setChannelValue(4, 0);

  for (int i = 0; i < 255; i++) {
    for (int j = 0; j < lamps; j++) {
      set_rgb_value(j, i, 255, 0, 0);
    }
    delay(5);
  }
  for (int i = 255; i > 0; i--) {
    for (int j = 0; j < lamps; j++) {
      set_rgb_value(j, i, 255, 0, 0);
    }
    delay(5);
  }
}
int tempSense() {
  int a = analogRead(pinTempSensor);

  float R = 1023.0 / a - 1.0;
  R = R0 * R;

  float temperature = 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15; // convert to temperature via datasheet
  return int(map(analogRead(0), 200, 600, 0, 255));
}

void iceTwinkle() {
  for (int i = 0; i < lamps; i++) {
    set_rgb_value(i, 0, 0, 0, 50);
  }
  if (random(10) % 3 == 0) {
    int which = random(lamps);
    set_rgb_value(which, 0, 0, 0, 255);
    delay(100);
    set_rgb_value(which, 0, 0, 0, 50);
  }

}
void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position = 0;

  for (int j = 0; j < 5; j++)
  {
    Position++; // = 0; //Position + Rate;
    for (int i = 0; i < 5; i++) {
      // sine wave, 3 offset waves make a rainbow!
      float level = sin(i + Position) * 127 + 128;
      float levels = cos(i + Position) * 127 + 128;
      //setPixel(i,level,0,0);
      set_rgb_value(i, 50, 0, levels, 0);
      //        //float level = sin(i+Position) * 127 + 128;
      //        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
      //                   ((sin(i+Position) * 127 + 128)/255)*green,
      //                   ((sin(i+Position) * 127 + 128)/255)*blue);
    }

    //showStrip();
    delay(WaveDelay);
  }
}


void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[lamps];
  int cooldown;
    for( int i = 0; i < lamps; i++) {
    cooldown = random(0, ((Cooling * 10) / lamps) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  for( int k= lamps - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  for( int j = 0; j < lamps; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  delay(60);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  byte t192 = round((temperature/255.0)*191);
 
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  if( t192 > 0x80) {                     // hottest
    set_rgb_value(Pixel, 255, heatramp, 0, 0);
    //setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
   set_rgb_value(Pixel, heatramp, 255, 0, 0);
    //setPixel(Pixel, 255, heatramp, 0);
  } else {                
    set_rgb_value(Pixel, heatramp, 200, 200, 0);// coolest
    //setPixel(Pixel, heatramp, 0, 0);
  }
}

