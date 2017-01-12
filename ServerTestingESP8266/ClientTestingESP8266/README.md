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

'''
#include <Wifi_S08_v2.h>

#define SSID "ESPeezy"
#define PASSWD "MOMENTUM"

#define POLLPERIOD 1000
#define BLOCKCHECK 100
'''

The include statement allows you to utilize the Wifi library we've provided.

The define statements are kind of like variables, you should only use them for recurring constants. The all uppercase is just a coding convention that is used when you define constants. 

After the include and define statements we see

'''
ESP8266 wifi = ESP8266(0,false);
'''

This is the initialization of a class. A class is like a predefined package. It has properties and functions. Read more about this in the Wifi Library Page. 
TODO: add link here

###Setup

In setup we generally want to do things that only need to be done once on boot up. 

Note that if you are defining variables this is not the place to do it. You should declare functions and variables before setup. Declaring is not the same as initialization.

---------
Declaring
---------
'''
int num;		//declaring a variable
int add(int num1, int num2); //declaring a function
'''
----------------------------------
Initialization/Assignment/Defining
----------------------------------

'''
num = 1;				//initializing/assigning a variable

// defining a function
int add(int num1, int num2){ 
	return num1 + num2;
}
'''

