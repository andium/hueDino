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

#include "hueDino.h"



hueDino::hueDino(WiFiClient& hueClient, const char* hueBridgeIP)
{
	_hueClient = hueClient;
	_hueBridgeIP = hueBridgeIP;	
}

uint8_t hueDino::begin(const char* userId)
{	
	_restclient = RestClient(_hueClient, _hueBridgeIP);
	_userId = userId;

	//Check for username
	if(_userId == "")
	{
		Serial.println();Serial.println();
		Serial.println("Missing username!");
		Serial.println("Have you ran the 'getUsername' sketch and authorized your app?");
		Serial.println("Don't forge to edit ---> #define hueUser \"yourUsernameHere\"   //ex: \"Ur80SKqRKO0ojlFInzdjAZEHy0kjYWznakufY60m\"");
		Serial.println("At the top of the sketch!");
		while(true){};
	}
	Serial.println();
	Serial.print("Starting hueDino - verifying connection to Phillips Hue Bridge at: ");
	Serial.println(_hueBridgeIP);
	
	//check for static hue webpage served by bridge to make sure
	//we have the correct IP address
	int status = 0;
	uint8_t counter = 0;
	while(status != 200)
	{	
		Serial.print("...");
		status = _restclient.get("/");
		delay(200);
		++counter;

		if(counter > 3)
		{
			Serial.print("Can't find Hue Bridge! Is ");
			Serial.print(_hueBridgeIP);
			Serial.println(" the correct IP address?");
		    while(true){};

		}
	}
	Serial.println("Connected!");
	return 0;
	
}

String hueDino::registerApp(String username)
{
	String endpoint = "/api";
	String body = "{\"devicetype\":\"";

	body += username;
	body += "\"}";

	_restclient.post(endpoint, body);

	return _restclient.readResponse();
}

String hueDino::lightOn(uint8_t lightId)
{	
	#ifdef DEBUG
		Serial.println();Serial.print("Light ID: ");
		Serial.print(lightId);Serial.println(" ON");
	#endif
    _restclient.put(_lightStateEndpoint(lightId), LIGHT_ON);
    return _restclient.readResponse();
}

String hueDino::lightOff(uint8_t lightId)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Light ID: ");
		Serial.print(lightId);Serial.println(" OFF");
	#endif
    _restclient.put(_lightStateEndpoint(lightId), LIGHT_OFF);
    return _restclient.readResponse();
}

String hueDino::groupOn(uint8_t groupId)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Group ID: ");
		Serial.print(groupId);Serial.println(" ON");
	#endif
    _restclient.put(_groupActionEndpoint(groupId), LIGHT_ON);
    return _restclient.readResponse();
}

String hueDino::groupOff(uint8_t groupId)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Group ID: ");
		Serial.print(groupId);Serial.println(" OFF");
	#endif
    _restclient.put(_groupActionEndpoint(groupId), LIGHT_OFF);
    return _restclient.readResponse();
}

String hueDino::brightness(uint8_t lightId, uint8_t brightness)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Light ID: ");
		Serial.print(lightId);Serial.print(" set brightness to ");
		Serial.println(brightness);
	#endif
	String bri = "{\"transitiontime\":0, \"bri\":";

	bri += String(brightness);
	bri += "}";

    _restclient.put(_lightStateEndpoint(lightId), bri);
    return _restclient.readResponse();
}

String hueDino::hue(uint8_t lightId, uint16_t setHue)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Light ID: ");
		Serial.print(lightId);Serial.print(" set hue to ");
		Serial.println(setHue);
	#endif
	String hue = "{\"transitiontime\":0, \"hue\":";

	hue += String(setHue);
	hue += "}";

    _restclient.put(_lightStateEndpoint(lightId), hue);
    return _restclient.readResponse();	
}

//Input range 0-254
String hueDino::sat(uint8_t lightId, uint8_t setSat)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Light ID: ");
		Serial.print(lightId);Serial.print(" set saturation to ");
		Serial.println(setSat);
	#endif
	String sat = "{\"transitiontime\":0, \"sat\":";

	sat += String(setSat);
	sat += "}";

    _restclient.put(_lightStateEndpoint(lightId), sat);
    return _restclient.readResponse();	
}

//Input Raneg 6500 to 2000
String hueDino::colorTemp(uint8_t lightId, uint16_t temp)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Light ID: ");
		Serial.print(lightId);Serial.print(" set temperature to ");
		Serial.println(temp);
	#endif
	uint16_t ct = map(temp, 2000, 6500, 153, 500); //map to hue API range 153-500
	String ctVal = "{\"transitiontime\":0, \"ct\":";

	ctVal += String(ct);
	ctVal += "}";

	if((ct > 500)||(ct < 153))
	{
		return "Temperature out of range, 2000K-6500K only!";
	}

	_restclient.put(_lightStateEndpoint(lightId), ctVal);
    
    return _restclient.readResponse();
}

String hueDino::colorLoop(uint8_t lightId, bool enable)
{	
	#ifdef DEBUG
		Serial.println();Serial.print("Light ID: ");
		Serial.print(lightId);Serial.print(" set color loop to ");
		Serial.println(enable);
	#endif
	if(enable)
	{
		_restclient.put(_lightStateEndpoint(lightId), COLOR_LOOP_ON);
	}
	else
	{
		_restclient.put(_lightStateEndpoint(lightId), COLOR_LOOP_OFF);
	}
    
    return _restclient.readResponse();
}

//alertState types: none, select, or lselect
String hueDino::alert(uint8_t lightId, String alertState)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Light ID: ");
		Serial.print(lightId);Serial.print(" set alert to ");
		Serial.println(alertState);
	#endif
	String alert = "{\"transitiontime\":0, \"alert\":\"";

	alert += alertState;
	alert += "\"}";

    _restclient.put(_lightStateEndpoint(lightId), alert);
    return _restclient.readResponse();
}

String hueDino::flash(uint8_t lightId)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Light ID: ");
		Serial.print(lightId);Serial.print(" set to flash ");
	#endif
    _restclient.put(_lightStateEndpoint(lightId), "{\"transitiontime\":0, \"alert\":\"select\"}");
    delay(800);
    return _restclient.readResponse();
}

String hueDino::getLightIds(void)
{
	StaticJsonDocument<4096> jsonBuffer; //may need to increase static buffer for large # lights
	String json = ""; 
	

	json = _getLights();
	deserializeJson(jsonBuffer, json);
	JsonObject root = jsonBuffer.as<JsonObject>();
    
	
	int j=0;

	for(JsonObject::iterator it=root.begin(); it!=root.end(); ++it)
	{
		lightIds[j] = it->key().c_str();
		lightNames[j] = jsonBuffer[lightIds[j]]["name"].as<String>();

		#ifdef DEBUG_IDs
			Serial.println();
			Serial.print("Light ID: ");
			Serial.println(lightIds[j]);
			Serial.print("Light Name: ");
			Serial.println(lightNames[j]);
		#endif

		j++; //number of lights
	}

	numLights = j;

	return json;
}

String hueDino::getGroupIds(void)
{
	StaticJsonDocument<4096> jsonBuffer;
	String json = ""; 
	

	json = _getGroups();
    deserializeJson(jsonBuffer, json);
	JsonObject root = jsonBuffer.as<JsonObject>();    
	
	int j=0;

	for(JsonObject::iterator it=root.begin(); it!=root.end(); ++it)
	{
		groupIds[j] = it->key().c_str();
		groupNames[j] = jsonBuffer[groupIds[j]]["name"].as<String>();

		#ifdef DEBUG_IDs
			Serial.println();
			Serial.print("Group ID: ");
			Serial.println(groupIds[j]);
			Serial.print("Group Name: ");
			Serial.println(groupNames[j]);
		#endif

		j++; //number of groups
	}

	numGroups = j;

	return json;
}

void hueDino::turnAllLightsOff(void)
{
	getGroupIds(); //update groupId arrays
	delay(1000); //For /groups commands you should keep to a maximum of 1 per second.

	for(int i=0; i<numGroups; i++)
	{
		groupOff(groupIds[i].toInt());
		delay(1000); //For /groups commands you should keep to a maximum of 1 per second.
		#ifdef DEBUG
		Serial.println();
		Serial.print("Turning off ");
		Serial.println(groupNames[i]);
		#endif
	}
}

void hueDino::turnAllLightsOn(void)
{
	getGroupIds(); //update groupId arrays
	delay(1000); //For /groups commands you should keep to a maximum of 1 per second.

	for(int i=0; i<numGroups; i++)
	{
		groupOn(groupIds[i].toInt());
		delay(1000); //For /groups commands you should keep to a maximum of 1 per second.
		#ifdef DEBUG
		Serial.println();
		Serial.print("Turning on ");
		Serial.println(groupNames[i]);
		#endif
	}
}

String hueDino::groupBrightness(uint8_t groupId, uint8_t brightness)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Group ID: ");
		Serial.print(groupId);Serial.print(" set brightness to ");
		Serial.println(brightness);
	#endif
	String bri = "{\"transitiontime\":0, \"bri\":";

	bri += String(brightness);
	bri += "}";

    _restclient.put(_groupActionEndpoint(groupId), bri);
    return _restclient.readResponse();
}

String hueDino::groupHue(uint8_t groupId, uint16_t setHue)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Group ID: ");
		Serial.print(groupId);Serial.print(" set hue to ");
		Serial.println(setHue);
	#endif
	String hue = "{\"transitiontime\":0, \"hue\":";

	hue += String(setHue);
	hue += "}";

    _restclient.put(_groupActionEndpoint(groupId), hue);
    return _restclient.readResponse();	
}

String hueDino::groupSat(uint8_t groupId, uint8_t setSat)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Group ID: ");
		Serial.print(groupId);Serial.print(" set saturation to ");
		Serial.println(setSat);
	#endif
	String sat = "{\"transitiontime\":0, \"sat\":";

	sat += String(setSat);
	sat += "}";

    _restclient.put(_groupActionEndpoint(groupId), sat);
    return _restclient.readResponse();	
}

String hueDino::groupColorLoop(uint8_t groupId, bool enable)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Group ID: ");
		Serial.print(groupId);Serial.print(" set color loop to ");
		Serial.println(enable);
	#endif
	if(enable)
	{
		_restclient.put(_groupActionEndpoint(groupId), COLOR_LOOP_ON);
	}
	else
	{
		_restclient.put(_groupActionEndpoint(groupId), COLOR_LOOP_OFF);
	}
    
    return _restclient.readResponse();
}

String hueDino::groupAlert(uint8_t groupId, String alertState)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Group ID: ");
		Serial.print(groupId);Serial.print(" set alert to ");
		Serial.println(alertState);
	#endif
	String alert = "{\"transitiontime\":0, \"alert\":\"";

	alert += alertState;
	alert += "\"}";

    _restclient.put(_groupActionEndpoint(groupId), alert);
    return _restclient.readResponse();
}

String hueDino::groupFlash(uint8_t groupId)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Group ID: ");
		Serial.print(groupId);Serial.print(" set to flash");
	#endif
	 _restclient.put(_groupActionEndpoint(groupId), "{\"transitiontime\":0, \"alert\":\"select\"}");
    delay(800);
    return _restclient.readResponse();
}

String hueDino::groupColorTemp(uint8_t groupId, uint16_t temp)
{
	#ifdef DEBUG
		Serial.println();Serial.print("Group ID: ");
		Serial.print(groupId);Serial.print(" set temperature to ");
		Serial.println(temp);
	#endif
	uint16_t ct = map(temp, 2000, 6500, 153, 500); //map to hue API range 153-500
	String ctVal = "{\"transitiontime\":0, \"ct\":";

	ctVal += String(ct);
	ctVal += "}";

	if((ct > 500)||(ct < 153))
	{
		return "Temperature out of range, 2000K-6500K only!";
	}

	_restclient.put(_groupActionEndpoint(groupId), ctVal);
    
    return _restclient.readResponse();
}

String hueDino::_lightStateEndpoint(uint8_t lightId)
{
	String endpoint = "/api/";

	endpoint += _userId;
	endpoint += "/lights/";
	endpoint += lightId;
	endpoint += "/state";

	return endpoint;
}

String hueDino::_groupActionEndpoint(uint8_t groupId)
{
	String endpoint = "/api/";

	endpoint += _userId;
	endpoint += "/groups/";
	endpoint += groupId;
	endpoint += "/action";

	return endpoint;
}

String hueDino::_getGroups(void)
{
	String endpoint = "/api/";

	endpoint += _userId;
	endpoint += "/groups";

	_restclient.get(endpoint);

	return _restclient.readResponse();
}

String hueDino::_getLights(void)
{
	String endpoint = "/api/";

	endpoint += _userId;
	endpoint += "/lights";

	_restclient.get(endpoint);

	return _restclient.readResponse();
}