/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "thereisnospoon";

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
char msg[512];
char incomingByte = 0;
String key = "a";

void keyupFunction() {
  key = "p";  
}

void keydownFunction() {
  key = "d";
}
 
void handleRoot() {
  server.send(200, "text/html", key);
}

void handleControlGet(){
  server.send(200, "text/html", "<html><body><p>Press and hold down a key inside the text field to set a red background color. Release the key to set a green background color.</p><input type=\"text\" id=\"demo\" onkeydown=\"keydownFunction()\" onkeyup=\"keyupFunction()\"><script>function keydownFunction() {document.getElementById(\"demo\").style.backgroundColor = \"red\";}function keyupFunction() {document.getElementById(\"demo\").style.backgroundColor = \"green\";}</script></body></html>");
}

void handleControlPost(){
    server.send(200, "text/html", msg);
}

void handleRoverGet(){
  if (server.hasArg(server.uri())){
    server.send(200, "text/html", server.arg(1)) ;
  }
  else{
    server.send(200, "text/html", "No Args") ;
    }
}

void handleRoverPost(){
    server.send(200, "text/html", msg);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/control", HTTP_POST, handleControlPost);
  server.on("/control", HTTP_GET, handleControlGet);
  server.on("/rover", HTTP_POST, handleRoverPost);
  server.on("/rover", HTTP_GET, handleRoverGet);
  server.begin();
  Serial.println("HTTP server started");
}
char strbuffer[512];
char* str_ptr = strbuffer;

void loop() {
  server.handleClient();
  if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();
                // say what you got:
                
                if (incomingByte != '\n' and incomingByte != '\r'){
                  Serial.print("I received: ");
                  Serial.println(incomingByte);
                  *str_ptr = incomingByte;
                  str_ptr++;
                  Serial.println(strbuffer);
                }
                else if(incomingByte == '\n')
                {
                sprintf(msg, "<h1>%s</h1>", strbuffer);
                while(str_ptr != strbuffer){
                  *str_ptr = NULL;
                  str_ptr--;  
                }
                }
                
  }
}
