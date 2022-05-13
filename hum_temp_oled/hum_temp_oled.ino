#include <dht11.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char* ssid = "Redmi";
const char* password = "abhi1234";
int notfirstentryFlag = 0;
String timeReadings;
String hrTime;
unsigned long unixT;

dht11 DHT11;
int humidity = 0;
int temperature = 0;
WiFiClient client;

#define DHT11PIN 5
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define pin 14 

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//Domain name with URL path or IP address with path
String serverName = "http://worldtimeapi.org/api/timezone/Asia/Kolkata";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.

signed long lastTime = 0;
// Timer set to 100 minutes (6000000)
unsigned long timerDelay = 6000000;
unsigned long timeSnap;
// Set timer to 5 seconds (5000)
//unsigned long timerDelay = 5000;

/*
   Function to convert unix time to human readable time
   ---------------------------------------------------
   @Parameter (unixT) Time in unix format
   @Return : string with ( Time in HH:MM:SS format)

*/
void changeTimetoHR(unsigned long t_unix, int temp, int humi)
{
  int ssT, mmT, hhT;
  int pmFlag = 0;
  String hhmmss = "";
  ssT = t_unix % 60;
  t_unix = t_unix / 60;
  mmT = t_unix % 60;
  t_unix = t_unix / 60;
  hhT = t_unix % 24;

  if (hhT > 11)
  {
    if (hhT == 12)
    {
      hhT = hhT + 12;
    }
    hhT = hhT - 12;
    pmFlag = 1;
  }
  else
  {
    if (hhT == 0)
    {
      hhT = 12;
    }
    pmFlag = 0;
  }

  String ss = (String)ssT;
  String mm = (String)mmT;
  String hh = (String)hhT;

  if (hh.toInt() < 10) {
    hh = "0" + hh;
  }
  if (mm.toInt() < 10) {
    mm = "0" + mm;
  }
  if (ss.toInt() < 10) {
    ss = "0" + ss;
  }

  hhmmss = hhmmss + hh;
  hhmmss = hhmmss + ":";
  hhmmss = hhmmss + mm;
  //hhmmss = hhmmss + ":";
  //hhmmss = hhmmss + ss;
  if (pmFlag == 1)
  {
    hhmmss = hhmmss + " PM";
  }
  else {
    hhmmss = hhmmss + " AM";
  }

    Serial.print("temp:");
    Serial.print(temp);
    Serial.print("    humi:");
    Serial.println(humi);
  
  Serial.println(hhmmss);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("IoTuneZ Technologies"));
  display.println(F("   Abhinand Suresh"));
  display.println(F(" "));
  display.println(F(" ---- CLOCK ----"));
  display.println(F(" "));
  display.println(hhmmss);
  display.print("Humidity: ");
  display.print(humi);
  display.println(" %");
  display.print("Temperature: ");
  display.print(temp);
   display.print(" *C");
  display.display();
  return;
}


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 pinMode(LED_BUILTIN,OUTPUT);
 
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  pinMode(LED_BUILTIN,OUTPUT);
  // Clear the buffer
  display.clearDisplay();
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(300);

  Serial.println("");
  Serial.print("Connecting.");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
    if (WiFi.status() == WL_NO_SSID_AVAIL)
    {
      Serial.println("WL_NO_SSID_AVAIL");
      break;
    }
    if (WiFi.status() == WL_CONNECT_FAILED)
    {
      Serial.println("WL_CONNECT_FAILED");
      break;
    }
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

/*
    HttpGETRequest - Execute the get request and returns the result as a string
    @serverName - url
    @return string containing the response
*/

String httpGETRequest(String url) {
  HTTPClient http;
  http.begin(client, url);
  // Your IP address with path or Domain name with URL path
  // http.connect(url,80);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "";
  payload = http.getString();
  Serial.print(payload);

  // Free resources
  http.end();
  return (payload);
}


void loop() {
  //Send an HTTP POST request every 10 minutes

  if (((millis() - lastTime) > timerDelay) || (!notfirstentryFlag))
  {
    notfirstentryFlag = 1;
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      timeReadings = httpGETRequest(serverName);
      timeSnap=millis()/1000;
      
      //Serial.println(timeReadings);

      JSONVar timefromApi = JSON.parse(timeReadings);
      JSON.typeof(timefromApi); //can be used to get the type of the variables

      if (JSON.typeof(timefromApi) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      Serial.print("JSON object = ");
      Serial.println(timefromApi);

      // myObject.keys() can be used to get an array of all the keys in the object
      JSONVar keys = timefromApi.keys();
      for (int i = 0; i < keys.length(); i++)
      {
        JSONVar value = timefromApi[keys[i]];
        Serial.print(keys[i]);
      }
      Serial.print("JSON.typeof(timefromApi) = ");
      Serial.println(JSON.typeof(timefromApi)); // prints: object

      // myObject.hasOwnProperty(key) checks if the object contains an entry for key
      if (timefromApi.hasOwnProperty("datetime")) {
        Serial.print("myObject[\"Key of date and time\"] = ");
        Serial.println((const char*)timefromApi["datetime"]);
      }

      if (timefromApi.hasOwnProperty("unixtime")) {
        Serial.print("Unix Time is: ");
        Serial.println((int)timefromApi["unixtime"]);
      unixT = (int)timefromApi["unixtime"] + 19800;
        
      }

    }
    else {
      Serial.println("Network Unavailable");
      //delay(5000);
    }
  }
//find temp and humidity

  int j;
  unsigned int loopCnt;
  int chr[40] = {0};
  unsigned long time1;
bgn:
  delay(2000);
  //Set interface mode 2 to: output
  //Output low level 20ms (>18ms)
  //Output high level 40μs
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delay(20);
  digitalWrite(pin, HIGH);
  delayMicroseconds(40);
  digitalWrite(pin, LOW);
  //Set interface mode 2: input
  pinMode(pin, INPUT);
  //High level response signal
  loopCnt = 10000;
  while (digitalRead(pin) != HIGH){
    if (loopCnt-- == 0){
      //If don't return to high level for a long time, output a prompt and start over
      Serial.println("HIGH");
      goto bgn;
    }
  }
  //Low level response signal
  loopCnt = 3000;
  while (digitalRead(pin) != LOW){
    if (loopCnt-- == 0){
      //If don't return low for a long time, output a prompt and start over
      Serial.println("LOW");
      goto bgn;
    }
  }
  //Start reading the value of bit1-40
  for (int i = 0; i < 40; i++){
    while (digitalRead(pin) == LOW){}
    //When the high level occurs, write down the time "time"
    time1 = micros();
    while (digitalRead(pin) == HIGH){}
    //When there is a low level, write down the time and subtract the time just saved
    //If the value obtained is greater than 50μs, it is ‘1’, otherwise it is ‘0’
    //And save it in an array
    if (micros() - time1  > 50){
      chr[i] = 1;
    } else {
      chr[i] = 0;
    }
  }

  //Humidity, 8-bit bit, converted to a value
  humidity = chr[0] * 128 + chr[1] * 64 + chr[2] * 32 + chr[3] * 16 + chr[4] * 8 + chr[5] * 4 + chr[6] * 2 + chr[7];
  //Temperature, 8-bit bit, converted to a value
  temperature = chr[16] * 128 + chr[17] * 64 + chr[18] * 32 + chr[19] * 16 + chr[20] * 8 + chr[21] * 4 + chr[22] * 2 + chr[23];
if(temperature>28)
{
  digitalWrite(LED_BUILTIN,LOW);
  }
  else
  {
    digitalWrite(LED_BUILTIN,HIGH);
    }
  
    lastTime = millis();
    changeTimetoHR(unixT+(millis()/1000)-timeSnap, temperature, humidity);
    delay(1000);
}
