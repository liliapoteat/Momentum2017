#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "ESPap";
const char *password = "thereisnospoon";

ESP8266WebServer server(80);

//root page can be accessed only if authentification is ok
void handleRoot(){
  Serial.println("Enter handleRoot");
  String header;
  String content = "<html><body><H2>hello, you successfully connected to esp8266!</H2><br>";
  if (server.hasHeader("User-Agent")){
    content += "the user agent used is : " + server.header("User-Agent") + "<br><br>";
  }
  content += "You can access this page until you <a href=\"/login?DISCONNECT=YES\">disconnect</a></body></html>";
  server.send(200, "text/html", content);
  return;
}

//no need authentification
void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
String args = "No Args";
void handleRover(){
  if (server.hasArg("COMMANDS")){
      args = server.arg("COMMANDS");
    }
   String content = "<html><body>"+args+"</body></html>";
   server.send(200, "text/html", content);
   if (server.hasArg("RECEIVED")){
      args = "No Args";
    }
   return;
  }
  
void handleControl(){
  if (server.hasArg("COMMANDS")){
      String cmds = "/Rover?COMMANDS=";
      cmds += server.arg("COMMANDS");
      server.sendHeader("Location",cmds);
      server.sendHeader("Cache-Control","no-cache");
      server.sendHeader("Set-Cookie","ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }
  String content = "<html><body><form action='/control' method='POST'>Enter a command sequence<br>";
  content += "Commands:<input type='text' name='COMMANDS' placeholder='f,l,r,b'><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form></body></html>" ;
  server.send(200, "text/html", content);
  }

void setup(void){
  Serial.begin(115200);
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.on("/Rover", handleRover);


  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
