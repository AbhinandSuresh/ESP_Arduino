#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "Redmi";
const char *password = "abhi1234";
const long utcOffsetInSeconds = 19800;  // UTC offset for +5:30
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting.");
  while ( WiFi.status() != WL_CONNECTED ) {
  delay (1000);
  Serial.print (".");
  }
  Serial.println("");
  Serial.println("Connected Successfully.");
  timeClient.begin();
}

void loop() {
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  
  int hrs=timeClient.getHours();
  if(hrs>12)
  {
  Serial.print(timeClient.getHours()-12);
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.print(timeClient.getSeconds());
  Serial.println(" PM");
  }
   else
  { 
    if(hrs==0)
    {
      hrs=12;
     }
  Serial.print(hrs);
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.print(timeClient.getSeconds());
  Serial.println(" AM");
  //Serial.println(timeClient.getFormattedTime());
  }
}
