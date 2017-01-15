# Wifi Library
You will find the wifi methods available to you here and how to use them.

## The ESP8266 class

At the beginning of all the sample wifi related code you have, you will notice some code that looks like this.

```
ESP8266 wifi = ESP8266(1,false);  //for server
```

This is the declaration of an ESP8266 object. If you are not familiar with classes, think of them as a package with some variables and functions.

Here is the break down

ESP8266 wifi - declares that we have an ESP8266 object called wifi.

ESP8266(1,false) - This is an instance of an ESP8266. It takes two arguments, an integer representing the mode (0 - Client, 1 - Server) and a boolean (true - debug on, false debug off).

The class has several useful functions that will be described below as either server functions, client functions or startup functions.

##Start up Functions

You have seen wifi.begin() in the setup of all your wifi related code.

```
.begin() - sets up the wifi chip in the coresponding mode.
```

There are several commands that need to initially be sent to the wifi chip to set it up in a mode. Calling .begin() takes care of that.

## Client Functions

```
.getMAC() - gives you the MAC address of your wifi chip (not needed to operate wifi)

.connectWifi(SSID, PASSWD) - Takes two strings, SSID and PASSWD. This function tells the wifi chip what network to connect to.

.isConnected() - returns a boolean telling you if you are connected to the desired network.

.hasResponse() - returns a boolean telling you if you have received a response from the server

.getResponse() - returns a String containing the response from the server

.isBusy() - returns a boolean telling you whether the wifi chip is busy or not

.sendRequest(METHOD,DOMAIN,PORT,PATH,PARAMS) - Sends a request to the server. METHOD = GET, PORT = 80, DOMAIN = "192.168.4.1"
```

Generally speaking the most important functions here are sendRequest() and getResponse(). They are the two functions that help will communicating over wifi.

sendRequest sends a request to the server. Arguments listed above are arguments that should just be kept constant. PARAMS is a String that should be used to send sensor data from the client to the server. PATH generally will just be "/" but if you are feeling adventurous you can set a different path. Note that you will need to use the setPage function on the server side to accomodate for your new path.

## Server Functions

```
.startserver(SSID, PASSWD) - sets up the server and network that you want to host on the wifi chip.

.setPage(path,html) - this is used to tell the server what to send when a client requests a certain path. If the client requests a non-existant page, they are sent an error message

.getData() - returns a string containing whatever was sent in PARAMS from the client request
```

Here setPage and getData will help you communicate with the client from the server. By consistently updating the page contents in your loop, you can send the client new commands, and the client can send you sensor data during each request.