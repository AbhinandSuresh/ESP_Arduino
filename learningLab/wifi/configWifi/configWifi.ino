/*Author: Abhinand Suresh*/
#include <EEPROM.h>
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

String ssid = "";
String pw="";
int ssidLength;
int pwLength;

void setup() {
  Serial.begin(74880); 
  EEPROM.begin(512);                                      //Initialize EEPROM
  
  Serial.println("");
  Serial.println("Enter c to config.");
  delay(3000);
  //while(!Serial.available());
  String buff=Serial.readString();
  buff.toLowerCase();
  
  if(buff.substring(0,1)=="c")                          //Configure and save to eeprom
  {
  
  Serial.println("************************************");
  Serial.println("*             ESP  8266            *");
  Serial.println("*   Node MCU Configuration Mode:   *");
  Serial.println("*        IoTuneZ Internship        *");
  Serial.println("*                SJ                *");
  Serial.println("************************************");
  
  Serial.print("Enter SSID:");
  while(!Serial.available());
  ssid=Serial.readString();
  Serial.println(" OK ");
  ssidLength = ssid.length();
  Serial.print("Enter password:");
  while(!Serial.available());
  pw=Serial.readString();
  Serial.println(" OK ");
  pwLength = pw.length();

  ssid=ssid.substring(0,ssidLength-1);
  pw=pw.substring(0,pwLength-1);
  
  //Serial.println(ssid);
  //Serial.println(pw);
  //EEPROM.put(0,ssid);
  //EEPROM.put(ssid.length+1,pw);

  EEPROM.write(0,ssidLength);
  EEPROM.write(1,pwLength);
  
for(int i=0;i<ssidLength-1;i++)         //loop upto string length of SSID
  {
    EEPROM.write(2+i,ssid[i]);        //Write one by one with starting address of 2
  }
for(int i=0;i<pwLength-1;i++)           //loop upto string length of password
  {
    EEPROM.write(1+ssidLength+i,pw[i]); //Write one by one with starting address after ssid
  }
  
  //EEPROM.write(0,ssid);
  EEPROM.commit();                       //Store data to EEPROM
  }

ssid = "";
pw="";

ssidLength=EEPROM.read(0);              //read back from e2prom
pwLength=EEPROM.read(1);

for(int i=0;i<ssidLength-1;i++)
{
  ssid=ssid+char(EEPROM.read(2+i));     // read ssid from eeprom
}
for(int i=0;i<pwLength-1;i++)
{
  pw=pw+char(EEPROM.read(1+ssidLength+i)); // read password from eeprom   addr: (2 header + ssidLength-1 ssid + i)
}
Serial.print("Connecting to ");
Serial.print(ssid);
Serial.println(".");
WiFi.begin(ssid,pw);                    // connect to wifi using ssid & password
Serial.println("Connecting... Please wait...");
while(WiFi.status()!=WL_CONNECTED)
{
  yield();
  if(WiFi.status()==WL_CONNECTED)
  {
  Serial.println("Connected");
  break;
  } 
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
 if(WiFi.status()==WL_CONNECTED)      // Display IP addr
  {
  IPAddress ip =WiFi.localIP();
  Serial.print("IP Address is:");
  Serial.println(ip);
  } 
}

void loop() {
  ; //nothing to loop
}
