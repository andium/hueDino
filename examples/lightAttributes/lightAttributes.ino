 /******************************************************************* 
  This example sketch quickly runs through the available single
  light attributes made available for modification via the
  hueDino Arduino library. To learn more about the different 
  configuratble attributes check out Philips Hue API documentation
  https://developers.meethue.com/documentation/lights-api#16_set_light_state
  
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

#define hueUser "WL65MOih1azZ3VIxsyOpGqIWQE60iUtryGHmh8Ax"      //ex: "Ur80SKqRKO0ojlFInzdjAZEHy0kjYWznakufY60m"
#define lightId 4


char ssid[] = "";       // your network SSID (name)
char pass[] = "";       // your network password
char hueBridge[] = "";  // hue bridge ip address ex: "192.168.1.3"

WiFiClient wifi;
hueDino hue = hueDino(wifi, hueBridge);

void setup() {
  Serial.begin(115200);

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

  //On
  hue.lightOn(lightId);
  delay(2000);

  //Off
  hue.lightOff(lightId);
  delay(2000);

  //Brightness (1-254) note: 1 is the most dimm but not off
  hue.lightOn(lightId);
  hue.brightness(lightId, 1);
  delay(2000);

  //Hue (0-65535) Both 0 and 65535 are red, 25500 is green and 46920 is blue.
  hue.brightness(lightId, 254);
  hue.hue(lightId, 46920);
  delay(2000);

  //Saturation (254- 0) is the most saturated (colored) and 0 is the least saturated (white).
  hue.sat(lightId, 254);
  delay(2000);

  //Color Temperature The Mired Color temperature of the light. 
  //2012 connected lights are capable of 2000K to 6500K
  //http://en.wikipedia.org/wiki/Mired
  hue.colorTemp(lightId, 2000);
  delay(2000);

  //Color Loop - just how it sounds - a loop of color transitions 
  //to end it just send false to enable
  hue.colorLoop(lightId, true);
  delay(2000); //worth the 10second wait

  //Alert - alertStates (none, select(one blink), lselect(multiple flash)
  //https://developers.meethue.com/documentation/core-concepts#some_extra_fun_stuff
  hue.alert(lightId, "lselect");
  delay(3000);

  //Flash - sinlge flash
  hue.flash(lightId);
  delay(2000);
  
  
}

void loop() {


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

