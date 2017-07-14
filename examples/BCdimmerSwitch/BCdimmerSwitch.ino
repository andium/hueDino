/*******************************************************************************

 Dimmer Switch
 ---------------------------------
 Modified example code written by Stefan Dzisiewski-Smith and Peter Krige
 to poll for a touch on an electrode, and then transmit over UART to a
 WiFi enabled Arduino. This example is used as a dimmer switch which
 controls Philips Hue connected lights. Check out the full project
 write up --> https://www.hackster.io/bcarbs/paint-your-dimmer-switch-on-the-wall-a0a3a1
 
 This work is licensed under a MIT license https://opensource.org/licenses/MIT
 
 Copyright (c) 2016, Bare Conductive
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*******************************************************************************/


#include <MPR121.h>
#include <Wire.h>
#define MPR121_ADDR 0x5C
#define MPR121_INT 4

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX

void setup(){ 
  Serial.begin(115200);
  mySerial.begin(115200);   
  Wire.begin();

  if(!MPR121.begin(MPR121_ADDR)){ 
    Serial.println("error setting up MPR121");  
    switch(MPR121.getError()){
      case NO_ERROR:
        Serial.println("no error");
        break;  
      case ADDRESS_UNKNOWN:
        Serial.println("incorrect address");
        break;
      case READBACK_FAIL:
        Serial.println("readback failure");
        break;
      case OVERCURRENT_FLAG:
        Serial.println("overcurrent on REXT pin");
        break;      
      case OUT_OF_RANGE:
        Serial.println("electrode out of range");
        break;
      case NOT_INITED:
        Serial.println("not initialised");
        break;
      default:
        Serial.println("unknown error");
        break;      
    }
    while(1);
  }
  MPR121.setInterruptPin(MPR121_INT);
  MPR121.setTouchThreshold(40);    // lower numbers make the board more sensitive
  MPR121.setReleaseThreshold(20);  // make sure that the release threshold is always 
                                   // lower than the touch threshold

  MPR121.updateAll();
  

}

void loop(){

  if(MPR121.touchStatusChanged()){
    MPR121.updateTouchData();  
      // run through all electrodes
      for(int i=0; i<=5; i++){
        if(MPR121.isNewTouch(i)){           
          Serial.print("Brightness: ");
          Serial.println(i);
          char val=5;
          itoa(i, &val, 10);
          mySerial.write(val);
          
        } 
      }
    }
  }
