#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
 
const char* ssid = "Redmi";
const char* pw =  "abhi1234";
 
WiFiServer espServer(80);
 
void setup() {
 
  Serial.begin(57600);
  WiFi.begin(ssid, pw);
  Serial.println("");
  Serial.println("Connecting to Network... Please Wait..");
  
  while (WiFi.status() != WL_CONNECTED)
  {
  yield();
  }
  
  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());
  espServer.begin();
}
 
void loop() {
 
  WiFiClient client = espServer.available();
  int flag=1;
  if (client==1) {
    while (client.connected()) {
      if (flag==1)
      {
      Serial.println("Client Connected.");
      Serial.println("Message(s) from the Client: ");
      flag=0;
      }
       while (client.available()>0) 
      {
        char c = client.read();
        Serial.print(c);
      }
      //yield();
    }
    client.stop();
    Serial.println("");
    Serial.println("####################");
    Serial.println("Client disconnected");
    Serial.println("####################");
    Serial.println("");
 flag =1;
  }
}
