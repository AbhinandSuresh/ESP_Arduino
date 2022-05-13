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

String ssid = "Redmi";
String pw="abhi1234";

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting... Please wait...");
  // put your setup code here, to run once:
WiFi.begin(ssid,pw);
while(WiFi.status()!=WL_CONNECTED)
{
  yield();
  if(WiFi.status()==WL_IDLE_STATUS)
  {
  Serial.println("IDLE");
  break;
  } 
  if (WiFi.status()==WL_NO_SSID_AVAIL)
  {
  Serial.println("WL_NO_SSID_AVAIL");
  break;
  }
  if (WiFi.status()==WL_CONNECT_FAILED)
  {
  Serial.println("WL_CONNECT_FAILED");
  break;
  delay(5000);
  }  
}
IPAddress ip =WiFi.localIP();
Serial.println("Connected");
Serial.print("IP Address is:");
Serial.println(ip);
}

void loop() {
  yield();

}
