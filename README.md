# hueDino

An Arduino library for WiFi101 connected devices implementing the [Philips Hue API](https://developers.meethue.com/philips-hue-api)! This library handles API calls to the Philips Hue Bridge, which in turn allows you to control your lights from your WiFi enabled Arduino! Now you can control your lights using triggers created by your own sensors, homemade interfaces, switches, or even remotely! 

Your Arduino can send requests to turn all of your lights off with one command, or set individual light attributes one by one. Functions have been written to manipulate attributes for the [/lights](https://developers.meethue.com/documentation/lights-api) and [/groups](https://developers.meethue.com/documentation/groups-api) endpoints of the Hue API. To learn more about the Hue API check out the [Philips Hue API documentation](https://developers.meethue.com/philips-hue-api). To learn more about how Philips Hue works check out [How Hue Works](https://www.developers.meethue.com/documentation/how-hue-works).

hueDino gives you the capability to configure for a single light or assigned group of lights...

* On/Off - also on/off for all lights attached to a bridge
* Hue
* Saturation
* Brightness
* Color Temperature
* Effects - Color Loop
* Alerts - Alert & Flash


# Getting Started

In order to get your Arduino talking to your Philips Hue Lights you're going to need a few things. First...
*   Your Arduino needs to be WiFi101 enabled (port for ESP8266 coming soon)
    *  Arduino MKR1000, Feather ATWINC1500, [AnduinoWiFi](https://store.andium.com), etc
*   Download and install the ['hueDino' library](https://github.com/andium/hueDino/archive/master.zip) in your IDE at Arduino>>libraries as well as [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
*   Test out and install your Hue lights using [Philips Hue App](http://www2.meethue.com/en-us/philipshueapp) to ensure everything is working properly first. Currently no endpoints are exposed in the library for registering new lights.
*   You'll need physical access to the Hue Bridge in order to complete the app registration process.

Let's get started!

# App Registration

To start communicating with the Hue Bridge you need to authenticate your Arduino(your application) with the bridge. In order to do so you must have physical access to the bridge. Essentially, the arduino requests a username hash, you press the button on the top of the bridge, and then request a username hash again. Your second request returns you this...

```
[
	{
		"success": {
			"username": "WCBrB5gCdiIULlmTCcrbXh1bLjXYDSBmVzx5N2Ue"
		}
	}
]
```
In order to communicate with the bridge you must set the hueUser variable at the top of the example sketches to your username.

In the example sketches you'll find a sketch named ['getUsername'](https://github.com/andium/hueDino/blob/master/examples/getUsername/getUsername.ino). Download this sketch and fill in your WiFi SSID and password. You'll also need to fill in the IP address of the Hue Bridge. There a few ways to get this. Either log in to your routers admin page (usually found at http://192.168.1.1) and check the attached/connected devices. Alternatively hop over to the [Philips Hue Bridge Discovery Guide](https://developers.meethue.com/documentation/hue-bridge-discovery) for more ideas to discover the bridge's IP using tools like UPnP

Once you've filled in those details, flash the sketch, and follow the instructions in the serial terminal. When prompted, press the button on the bridge. Return to your serial terminal and send some data back to the Arduino to break the wait loop. (Literally send anything, a carriage return, a secret message, doesn't matter) This triggers the Arduino to send a second request for your username, which should then return the above success message. Save the hash in a safe spot! You'll need it for the other example sketches.

That's it! Now you have everything you need to start changing the state of your lights using your Arduino!

# Blinky 

The defacto 'Hello World' of embedded hardware projects. [Here's hueDino's spin](https://github.com/andium/hueDino/blob/master/examples/hueBlinky/hueBlinky.ino) on the must have example sketch. Just fill in your details from the 'getUsername' sketch and voila! 
```
#define hueUser ""      //ex: "Ur80SKqRKO0ojlFInzdjAZEHy0kjYWznakufY60m"

char ssid[] = "";       // your network SSID (name)
char pass[] = "";       // your network password
char hueBridge[] = "";  // hue bridge ip address ex: "192.168.1.3"
```
All the hue lights in your house will start blinking, turning on for 3 seconds, and then off for 3 seconds. The 'hueBlinky' sketch queries the bridge for all registered 'Groups' and then successively toggles all the lights.


# Things you should know...

* Totally regeret that state you just put our lights in? Unplug your Arduino, and toggle the power to your lights to reset.
* You can safely send about 10 /lights endpoint commands in a row before things get strange.
* You can only send 1 /groups targeted command per second! Be careful not to overload the bridge.
* A list of other interesting design decisions and intricacies of the Hue Lights API [can be found here.](https://developers.meethue.com/things-you-need-know)


## Coming Soon
* Ability to submit custom JSON to /groups and /lights endpoints from within Arduino sketch
  * ex: ``` sendRequestToLights(lightId, "{\"on\":true,\"hue\":50000,\"brightness\":200,\"sat\":254}");```
* ESP8266 Compatibility
* More global attribute functions for changing attributes accross all groups

              

