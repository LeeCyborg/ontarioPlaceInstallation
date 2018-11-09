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
#define DMX_MASTER_CHANNELS   50
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


//
// A little macro that takes a percentage and chnages it to 0-255
//
#define MAX_VALUE 255
#define PER_TO_VAL(percent) ((MAX_VALUE/100) * percent)


typedef enum DmxCh {
  WHITE  = 0,
  RED    = 1,
  GREEN  = 2,
  BLUE   = 3

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
  if (analogRead(0) > 550) {
    //supacold();
    pwarm();
  } else if (analogRead(0) < 550 && analogRead(0) > 400) {
    kindaCold();
  } else {
    supacold();
  }
}


void set_rgb_value(int lampNumber, int red, int green, int blue, int white) {
  int channelNumber = lampNumber * 4;

  // Set each of the colors
  dmx_master.setChannelValue(channelNumber + RED,   red  );
  dmx_master.setChannelValue(channelNumber + GREEN, green);
  dmx_master.setChannelValue(channelNumber + BLUE,  blue );
  dmx_master.setChannelValue(channelNumber + WHITE,  white );

}

void supacold() {
  dmx_master.setChannelValue(4, 255);

  for (int i = 0; i < 255; i++) {
    dmx_master.setChannelValue(2, i + random(50));
    dmx_master.setChannelValue(3, tempSense());
    delay(10);
  }
  for (int i = 255; i > 0; i--) {
    dmx_master.setChannelValue(2, i + random(50));
    dmx_master.setChannelValue(3, tempSense());
    delay(10);
  }
}
void kindaCold() {
  dmx_master.setChannelValue(3, 0);

  for (int i = 0; i < 255; i++) {
    dmx_master.setChannelValue(2, i);
    dmx_master.setChannelValue(4, 255);
    delay(20);
  }
  for (int i = 255; i > 0; i--) {
    dmx_master.setChannelValue(2, i);
    dmx_master.setChannelValue(4, 255);
    delay(20);
  }
}
void pwarm() {
  dmx_master.setChannelValue(4, 0);

  for (int i = 0; i < 255; i++) {
    dmx_master.setChannelValue(2, i);
    dmx_master.setChannelValue(3, 255);
    delay(5);
  }
  for (int i = 255; i > 0; i--) {
    dmx_master.setChannelValue(2, i);
    dmx_master.setChannelValue(3, 255);
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

