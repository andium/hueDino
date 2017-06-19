#ifndef __hueDino_H
#define __hueDino_H

#include <Arduino.h>
#include "Client.h"
#include "RestClient.h"
#include "ArduinoJson.h"
#include "WiFi101.h"

#define LIGHT_ON "{\"on\":true}"
#define LIGHT_OFF "{\"on\":false}"
#define COLOR_LOOP_ON "{\"transitiontime\":0, \"effect\":\"colorloop\"}"
#define COLOR_LOOP_OFF "{\"transitiontime\":0, \"effect\":\"none\"}"
#define MAX_LIGHTS_PER_BRIDGE 50
#define MAX_GROUPS_PER_BRIDGE 64

#define DEBUG

class hueDino {

public:
	String lightIds[MAX_LIGHTS_PER_BRIDGE] = {}; //50 max# of lights per bridge
	String lightNames[MAX_LIGHTS_PER_BRIDGE] = {}; //50 max# of lights per bridge
	String groupIds[MAX_GROUPS_PER_BRIDGE] = {}; //64 max# of groups per bridge
	String groupNames[MAX_GROUPS_PER_BRIDGE] = {}; //64 max# of groups per bridge
	uint8_t numGroups = 0;
	uint8_t numLights = 0;

	hueDino(WiFiClient& hueClient, const char* hueBridgeIP);
	void begin(const char* userId);

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
