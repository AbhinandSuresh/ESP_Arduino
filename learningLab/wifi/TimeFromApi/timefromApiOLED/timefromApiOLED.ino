/*
 * regularly fetching data.
 * may be a chance of mallicious activity detection bt server
 * author:AbhinandSuresh
*/

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



const char *ssid     = "Redmi";
const char *password = "abhi1234";

const long utcOffsetInSeconds = 19800;  // UTC offset for +5:30

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

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
  
   // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(300);
}

void loop() {
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  
  int hrs=timeClient.getHours();
  if(hrs>12 || hrs==12)
  {
     if(hrs==12)
    {
      hrs=24;
     }
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
 
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("IoTuneZ Technologies"));     
  display.println(F("Internship"));
  display.println(F("----------------"));
  display.println(F("ABHINAND SURESH")); 
  display.print(F(" "));
  String tym= timeClient.getFormattedTime();
     display.print(tym);
      display.print(F(" - "));
     String day = daysOfTheWeek[timeClient.getDay()];
     display.println(day);
     display.println(F(" "));
     display.println(F("----------------"));
     display.display(); 

}
