 /******************************************************************* 
  The typical 'hello world' of embedded development, blinky! In this 
  example sketch the hueDino library queries the Hue Bridge for all 
  established groups. If groups are found it then toggles all groups 
  on, and then off, every 3 seconds. The sequence of On and Off 
  states are printed to the terminal if '#define DEBUG' is uncommented 
  in hueDino.h.
  
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
}

void loop() {

  //Blinky!
  hue.turnAllLightsOn();  //turns all hue lights on
  delay(3000);
  hue.turnAllLightsOff(); //turns all hue lights off
  delay(3000);

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

