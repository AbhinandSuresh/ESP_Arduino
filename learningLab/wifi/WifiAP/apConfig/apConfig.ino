#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>

AsyncWebServer server(80);
String ssid = "NodeMCU";
String pw = "abcdefgh";
String rxpw ="";
String rxssid ="";
const char* PARAM_INPUT_1 = "ssidNew";
const char* PARAM_INPUT_2 = "pwNew";
int ssidLength;
int pwLength;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP_8266 Config </title>
  <H1>ESP_8266 Configuration Portal</H1><H2>IoTuneZ Technologies Internship</H2><br>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    Enter new SSID & Submit: <input type="text" name="ssidNew">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Enter Password & Submit: <input type="text" name="pwNew">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {

  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, pw);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);


     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
     {
     request->send_P(200, "text/html", index_html);
     }
     );

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      rxssid=inputMessage;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      rxpw=inputMessage;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "ESP Node MCU configured with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  
  server.onNotFound(notFound);
  server.begin();
  Serial.println("HTTP server started");

  ssidLength= rxssid.length();
  pwLength=rxpw.length();
  EEPROM.write(0,ssidLength);
  EEPROM.write(1,pwLength);
  
for(int i=0;i<ssidLength-1;i++)         //loop upto string length of SSID
  {
    EEPROM.write(2+i,rxssid[i]);        //Write one by one with starting address of 2
  }
for(int i=0;i<pwLength-1;i++)           //loop upto string length of password
  {
    EEPROM.write(1+ssidLength+i,rxpw[i]); //Write one by one with starting address after ssid
  }
  
  //EEPROM.write(0,ssid);
  EEPROM.commit();                       //Store data to EEPROM

String readssid = "";
String readpw="";

ssidLength=EEPROM.read(0);              //read back from e2prom
pwLength=EEPROM.read(1);

for(int i=0;i<ssidLength-1;i++)
{
  readssid=readssid+char(EEPROM.read(2+i));     // read ssid from eeprom
}
for(int i=0;i<pwLength-1;i++)
{
  readpw=readpw+char(EEPROM.read(1+ssidLength+i)); // read password from eeprom   addr: (2 header + ssidLength-1 ssid + i)
}
Serial.print("Connecting to ");
Serial.print(ssid);
Serial.println(".");
WiFi.begin(readssid,readpw);                    // connect to wifi using ssid & password
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
  ; //Nothing here
}
