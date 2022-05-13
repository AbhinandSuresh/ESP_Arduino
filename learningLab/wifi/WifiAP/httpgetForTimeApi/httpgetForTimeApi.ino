#include<time.h>

#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

const char* ssid = "Redmi";
const char* password = "abhi1234";
int notfirstentryFlag = 0;
String timeReadings;
WiFiClient client;

//Domain name with URL path or IP address with path
String serverName = "http://worldtimeapi.org/api/timezone/Asia/Kolkata";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.

signed long lastTime = 0;
// Timer set to 10 minutes (600000)
unsigned long timerDelay = 600000;

// Set timer to 5 seconds (5000)
//unsigned long timerDelay = 5000;


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

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


void loop() 
{
  //Send an HTTP POST request every 10 minutes
  
  if ( (!notfirstentryFlag) || ((millis() - lastTime) > timerDelay))
  {
    
    notfirstentryFlag=1;
    
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      timeReadings = httpGETRequest(serverName);
      
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
    Serial.print("myObject[\"datetime\"] = ");

    Serial.println((const char*)timefromApi["datetime"]);
  }
    }
    else {
      Serial.println("Please Wait for next update...");
      delay(150000);
    }
  }
    lastTime = millis();
}
