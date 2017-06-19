#include "hueDino.h"



hueDino::hueDino(WiFiClient& hueClient, const char* hueBridgeIP)
{
	_hueClient = hueClient;
	_hueBridgeIP = hueBridgeIP;	
}

void hueDino::begin(const char* userId)
{	
	_restclient = RestClient(_hueClient, _hueBridgeIP);
	_userId = userId;
	#ifdef DEBUG
		Serial.println();
		Serial.print("Starting hueDino - connecting to Phillips Hue Bridge at: ");
		Serial.println(_hueBridgeIP);
	#endif
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

String hueDino::lightOn(uint8_t lightId)
{
    _restclient.put(_lightStateEndpoint(lightId), LIGHT_ON);
    return _restclient.readResponse();
}

String hueDino::lightOff(uint8_t lightId)
{
    _restclient.put(_lightStateEndpoint(lightId), LIGHT_OFF);
    return _restclient.readResponse();
}

String hueDino::groupOn(uint8_t groupId)
{
    _restclient.put(_groupActionEndpoint(groupId), LIGHT_ON);
    return _restclient.readResponse();

}

String hueDino::groupOff(uint8_t groupId)
{
    _restclient.put(_groupActionEndpoint(groupId), LIGHT_OFF);
    return _restclient.readResponse();
}

String hueDino::brightness(uint8_t lightId, uint8_t brightness)
{
	String bri = "{\"transitiontime\":0, \"bri\":";

	bri += String(brightness);
	bri += "}";

    _restclient.put(_lightStateEndpoint(lightId), bri);
    return _restclient.readResponse();
}

String hueDino::hue(uint8_t lightId, uint16_t setHue)
{
	String hue = "{\"transitiontime\":0, \"hue\":";

	hue += String(setHue);
	hue += "}";

    _restclient.put(_lightStateEndpoint(lightId), hue);
    return _restclient.readResponse();	
}

//0-254
String hueDino::sat(uint8_t lightId, uint8_t setSat)
{
	String sat = "{\"transitiontime\":0, \"sat\":";

	sat += String(setSat);
	sat += "}";

    _restclient.put(_lightStateEndpoint(lightId), sat);
    return _restclient.readResponse();	
}

//6500K to 2000K
String hueDino::colorTemp(uint8_t lightId, uint16_t temp)
{
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

//none, select, or lselect
String hueDino::alert(uint8_t lightId, String alertState)
{
	String alert = "{\"transitiontime\":0, \"alert\":\"";

	alert += alertState;
	alert += "\"}";

    _restclient.put(_lightStateEndpoint(lightId), alert);
    return _restclient.readResponse();
}

String hueDino::flash(uint8_t lightId)
{
    _restclient.put(_lightStateEndpoint(lightId), "{\"transitiontime\":0, \"alert\":\"select\"}");
    delay(800);
    return _restclient.readResponse();
}

String hueDino::getLightIds(void)
{
	StaticJsonBuffer<4096> jsonBuffer;
	String json = ""; 
	

	json = _getLights();
	JsonObject& root = jsonBuffer.parseObject(json);
    
	
	int j=0;

	for(JsonObject::iterator it=root.begin(); it!=root.end(); ++it)
	{
		lightIds[j] = it->key;
		lightNames[j] = root[lightIds[j]]["name"].asString();

		#ifdef DEBUG
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
	StaticJsonBuffer<4096> jsonBuffer;
	String json = ""; 
	

	json = _getGroups();
	JsonObject& root = jsonBuffer.parseObject(json);
    
	
	int j=0;

	for(JsonObject::iterator it=root.begin(); it!=root.end(); ++it)
	{
		groupIds[j] = it->key;
		groupNames[j] = root[groupIds[j]]["name"].asString();

		#ifdef DEBUG
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
	String bri = "{\"transitiontime\":0, \"bri\":";

	bri += String(brightness);
	bri += "}";

    _restclient.put(_groupActionEndpoint(groupId), bri);
    return _restclient.readResponse();
}

String hueDino::groupHue(uint8_t groupId, uint16_t setHue)
{
	String hue = "{\"transitiontime\":0, \"hue\":";

	hue += String(setHue);
	hue += "}";

    _restclient.put(_groupActionEndpoint(groupId), hue);
    return _restclient.readResponse();	
}

String hueDino::groupSat(uint8_t groupId, uint8_t setSat)
{
	String sat = "{\"transitiontime\":0, \"sat\":";

	sat += String(setSat);
	sat += "}";

    _restclient.put(_groupActionEndpoint(groupId), sat);
    return _restclient.readResponse();	
}

String hueDino::groupColorLoop(uint8_t groupId, bool enable)
{
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
	String alert = "{\"transitiontime\":0, \"alert\":\"";

	alert += alertState;
	alert += "\"}";

    _restclient.put(_groupActionEndpoint(groupId), alert);
    return _restclient.readResponse();
}

String hueDino::groupFlash(uint8_t groupId)
{
	 _restclient.put(_groupActionEndpoint(groupId), "{\"transitiontime\":0, \"alert\":\"select\"}");
    delay(800);
    return _restclient.readResponse();
}

String hueDino::groupColorTemp(uint8_t groupId, uint16_t temp)
{
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











