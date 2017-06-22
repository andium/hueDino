/*******************************************************************
 Check out the included Arduino sketches and the getting started 
 guide here! 
 https://github.com/andium/hueDino
 
 This is an Arduino implementation of the Philips Hue API.
 It currently supports the critical API endpoints necessary
 for registering an application with the Hue Bridge and changing 
 light or group attributes. This library is tightly coupled to the 
 WiFi101 library, which means it will work great with the Arduino MKR1000, 
 Adafruit Feather MO w/ the ATWINC1500, AnduinoWiFi shield 
 or anywhere the WiFi101 library is supported. A slightly modified version
 of Mario Banzi's RestClient is included with src and is used to simplify
 the RESTful calls to the Hue API.
 https://github.com/arduino-libraries/RestClient
 Json parsing is provided via ArduinoJson, thanks bblanchon! Be sure to 
 clone and install his library prior to trying this out!
 https://github.com/bblanchon/ArduinoJson
 
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

#ifndef __hueDino_H
#define __hueDino_H

#include <Arduino.h>
#include "Client.h"
#include "./include/RestClient.h" 
#include "ArduinoJson.h"
#include "WiFi101.h"

#define LIGHT_ON "{\"on\":true}"
#define LIGHT_OFF "{\"on\":false}"
#define COLOR_LOOP_ON "{\"transitiontime\":0, \"effect\":\"colorloop\"}"
#define COLOR_LOOP_OFF "{\"transitiontime\":0, \"effect\":\"none\"}"
#define MAX_LIGHTS_PER_BRIDGE 50
#define MAX_GROUPS_PER_BRIDGE 64

#define DEBUG
/*#define DEBUG_IDs*/

class hueDino {

public:
	String lightIds[MAX_LIGHTS_PER_BRIDGE] = {}; //50 max# of lights per bridge
	String lightNames[MAX_LIGHTS_PER_BRIDGE] = {}; 
	String groupIds[MAX_GROUPS_PER_BRIDGE] = {}; //64 max# of groups per bridge
	String groupNames[MAX_GROUPS_PER_BRIDGE] = {}; 
	uint8_t numGroups = 0;
	uint8_t numLights = 0;

	hueDino(WiFiClient& hueClient, const char* hueBridgeIP);
	uint8_t begin(const char* userId);
	String registerApp(String username);

	/*Single /light endpoint methods*/
	String lightOn(uint8_t lightId);
	String lightOff(uint8_t lightId);
	String brightness(uint8_t lightId, uint8_t brightness);
	String hue(uint8_t lightId, uint16_t setHue);
	String sat(uint8_t lightId, uint8_t setSat);
	String colorLoop(uint8_t lightId, bool enable);
	String alert(uint8_t lightId, String alertState);
	String flash(uint8_t lightId);
	String colorTemp(uint8_t lightId, uint16_t temp);
	String getLightIds(void);

	/*When updating light attributes, unless there are a dozen or more lights it is generally more 
	efficient to use the lights API. With larger numbers of lights, it can be more efficient to access 
	them as a group, using the groups API.*/

	/* /groups endpoint methods */
	void turnAllLightsOn(void);
	void turnAllLightsOff(void);
	String groupOn(uint8_t groupId);
	String groupOff(uint8_t groupId);
	String getGroupIds(void);
	String groupBrightness(uint8_t groupId, uint8_t brightness);
	String groupHue(uint8_t groupId, uint16_t setHue);
	String groupSat(uint8_t groupId, uint8_t setSat);
	String groupColorLoop(uint8_t groupId, bool enable);
	String groupAlert(uint8_t groupId, String alertState);
	String groupFlash(uint8_t groupId);
	String groupColorTemp(uint8_t groupId, uint16_t temp);

private:

	const char* _userId;
	const char* _hueBridgeIP;

    WiFiClient _hueClient;
    RestClient _restclient;
    
	String _lightStateEndpoint(uint8_t lightId);
	String _groupActionEndpoint(uint8_t groupId);
	String _getLights(void);
	String _getGroups(void);
};


#endif /*__hueDino_H */
