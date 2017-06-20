 /******************************************************************* 
  Initial sketch used to authenticate your application with the 
  Philips Hue Bridge. Open terminal to retrieve instructions and 
  your username hash which can be used for further API interaction.
  
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

#define hueUser ""          //in future sketches save your username here
#define appName "myHueApp"  //your app's name

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
  
  //Register your app w/ the Hue Bridge
  hue.registerApp(appName);
  
  //-------------------------------------------------------------------------
  Serial.println();
  Serial.println("Press the button on the center of the Philips Hue Bridge.");
  Serial.println();
  Serial.println("By confirming phyisical access you're authorizing your app to communicate with the bridge");
  Serial.println("Once you've completed this process send a message over serial to your Arduino.");
  Serial.println("Just type anything and hit 'send' or the Enter key.");
  Serial.println();
  
  while (Serial.available() <= 0) 
  {
    delay(300);
    //delay loop which waits giving you a chance to press the Hue Bridge button
    //Sending serial data to your Arduino will break the loop
  }
  
  Serial.println(hue.registerApp(appName));
  Serial.println("You should've received a message like... [{\"success\":{\"username\":\"bkZV7kAKiERnDi...\"}}]");
  Serial.println("Copy down this username hash in a safe spot! You'll edit the '#define hueUser' with your username in future examples");
  //-------------------------------------------------------------------------

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
