# WifiClient Example

This code uses the ESP8266 to communicate via wifi. In this example, we are set up as a client which can speak to a server.

## Before Using

* Have you made the approriate changes serial1.c?
	* If not see [Joe's Tutorial](https://iesc-s1.mit.edu/2017/momentum)
* Have you wired your circuit properly? TODO: See Diagram
* Have you looked at the functions provided by the Wifi_S08_v2 library? TODO: See Library

## Code Break Down

### Syntax Clarification

At the top of the code you will find some include and define statements.

```
#include <Wifi_S08_v2.h>

#define SSID "ESPeezy"
#define PASSWD "MOMENTUM"

#define POLLPERIOD 1000
```

The include statement allows you to utilize the Wifi library we've provided.

The define statements are kind of like variables, you should only use them for recurring constants. The all uppercase is just a coding convention that is used when you define constants. 

After the include and define statements we see

```
ESP8266 wifi = ESP8266(0,false);
```

This is the initialization of a class. A class is like a predefined package. It has properties and functions. Read more about this in the Wifi Library Page. 
TODO: add link here

###Setup

In setup we generally want to do things that only need to be done once on boot up. 

Note that if you are defining variables this is not the place to do it. You should declare functions and variables before setup. Declaring is not the same as initialization.


Declaring:

```
int num;		//declaring a variable
int add(int num1, int num2); //declaring a function
```

Initialization/Assignment/Defining:


```
num = 1;				//initializing/assigning a variable

// defining a function
int add(int num1, int num2){ 
	return num1 + num2;
}
```
Initialization and assignment can happen in the setup. You would generally define your function after loop so it wouldn't be in the way.

For the setup in this code, you should see this

```
Serial.begin(115200);
wifi.begin();
MAC = wifi.getMAC();
wifi.connectWifi(SSID, PASSWD);
while (!wifi.isConnected()); //wait for connection

```

The Serial.begin() function sets things up so that you can communication with the Teensy via USB Serial. This is what lets you open up a serial monitor. Serial in general is useful for debugging. The number in the parenthesis just sets the transmission speed, often refered to as the baud rate. All you need to know here is that your serial monitor should be set to the same speed.

All the lines about wifi are mentioned in the Wifi Library Page. TODO: insert link

Lastly, there is the pin setup. This is used only for digital pins. In this mode the pins will only output/input a HIGH or a LOW voltage value. LOW-0, HIGH-3.3~5V

###Loop

The last part of the code is what will be the most challenging part for most people. This requires that you use the Wifi Library. 

```
if (wifi.hasResponse()) {
    String response = wifi.getResponse();
    Serial.print("RESPONSE: ");
    Serial.println(response);
    count++;
    Serial.println(count);
  }

if (!wifi.isBusy() && millis()-lastRequest > POLLPERIOD) {
    String domain = "192.168.4.1";
    String path = "/"; 
    
    wifi.sendRequest(GET, domain, 80, path, "");
    lastRequest = millis();
  }
```

The game plan here is fairly straight forward. Try looking at the second if statement first. We check if the wifi is busy and if we've waited long enough since the last request. Here POLLPERIOD is used to decide how long we wait.

Assuming we enter that if statement, we see domain, and path. The domain for us should always be the same as written above unless told otherwise. The path is the page you would like to visit within the domain. You can learn more about how this is relevant in the Wifi library. TODO: add link. Continuing in the code, we send a request and declare that we timestamp it by setting last Request to millis()

Now back to the first if statement. This checks if a response was received. Once we do have a response, we print it and increment count; a variable that counts the number of reponses that have been received.

And that's it for the basic client!