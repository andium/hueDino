 /******************************************************************* 
  This example sketch listens on a serial port for incoming brightness
  values. This pairs well with the conductive paint dimmer swtich exmple. 
  'BCdimmerSwitch.ino'
  To learn more about controlling your Philips hue lights with 
  conductive paint check out this write up on hackster.
  https://www.hackster.io/bcarbs/paint-your-dimmer-switch-on-the-wall-a0a3a1
  
  Check out the included Arduino sketch examples to get started!
  https://github.com/andium/hueDino

  Written by Brian Carbonette Copyright © 2017 Andium 
  
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
 
  http://www.apache.org/licenses/LICENSE-2.0
 
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 *******************************************************************/
 
#include "hueDino.h"

#define hueUser ""      //ex: "Ur80SKqRKO0ojlFInzdjAZEHy0kjYWznakufY60m"
#define lightId 5


char ssid[] = "";       // your network SSID (name)
char pass[] = "";       // your network password
char hueBridge[] = "";  // hue bridge ip address ex: "192.168.1.3"

WiFiClient wifi;
hueDino hue = hueDino(wifi, hueBridge);
int level = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);

  connectToWiFi();

  //Start Hue
  hue.begin(hueUser);

  /*Uncomment the section below if you're unsure of the ID of the 
   *light you'd like to control
   *
   */
  
  /*//Query bridge for available lights
  Serial.println(hue.getLightIds()); //most hueDino methods return strings containing the raw json responses
                                     //just encapsulate your function calls inside Serial.print statements to 
  Serial.println();                  //to print them to the screen
  Serial.print("hueDino found: ");
  Serial.print(hue.numLights);
  Serial.println(" connected lights");
  Serial.println();
  Serial.println("Available Light IDs: ");
  
  for(int i=0; i<hue.numLights; i++)
  {
      Serial.print(hue.lightIds[i]);
      Serial.print(",");
  }*/


  /*Available attributes/states
   *on,off,brightness,hue,saturation,colorTemp,colorLoop,alert,flash
   *reference: https://developers.meethue.com/documentation/lights-api
   *settings are persistent and will not revert after sketch has run
   */

  //On - make surethe light's are on before we try to dimm
  hue.lightOn(lightId);
  delay(200);

}

void loop() {

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    level = Serial1.read();  
    Serial.println(level);

    switch (level) {
      case 48:
      hue.brightness(lightId, 250);
      break;
    case 49:
      hue.brightness(lightId, 200);
      break;
    case 50:
      hue.brightness(lightId, 150);
      break;
    case 51:
      hue.brightness(lightId, 100);
      break;
    case 52:
      hue.brightness(lightId, 50);
      break;
    case 53:
      hue.brightness(lightId, 1);
      break;    
    default: 
      hue.alert(lightId, "lselect");
    break;
  }
  }
  
}

void connectToWiFi()
{
  int status = WL_IDLE_STATUS;
   
  while(status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  
}

