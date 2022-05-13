#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>

AsyncWebServer server(80);
String ssid = "ESP8266_NodeMCU";
String pw = "12345678";
String received_pw ="";
String received_ssid ="";
const char* PARAM_INPUT_1 = "ssidNew";
const char* PARAM_INPUT_2 = "pwNew";
int ssidLength=0;
int pwLength=0;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP_8266 Config </title>
  <H1>ESP_8266 NODE MCU Configuration Portal</H1><H2><p >IoTuneZ Technologies Internship</H2><br>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><BODY BG color=\"YELLOW\">
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
  request->send(404, "text/plain", "Error!!! Page Not found");
}

void setup() {
  flag: delay(1000);
  EEPROM.begin(512);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point..., Please Wait");
  WiFi.softAP(ssid, pw);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point IP address is: ");
  Serial.println(myIP);

     server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
     {
     request->send_P(200, "text/html", index_html);
     }
     );

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1="";
    String inputMessage2="";
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
     
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) 
    {
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
    }
    else 
    {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
      inputParam = "none";
    }
    received_ssid=inputMessage1;
    received_pw=inputMessage2;
    
    if(received_ssid.length())
    {
      ssidLength= received_ssid.length();
      EEPROM.write(0,ssidLength);
      for(int i=0;i<ssidLength;i++)         //loop upto string length of SSID
      {
        EEPROM.write(2+i,received_ssid[i]); //Write one by one with starting address of 2
      }
        EEPROM.commit();
    }
     if(received_pw.length())
     {
      pwLength= received_pw.length();
      EEPROM.write(1,pwLength);
      for(int i=0;i<pwLength;i++)           //loop upto string length of password
      {
        EEPROM.write(2+ssidLength+i,received_pw[i]);
        // Serial.println(received_pw[i]);            //Write one by one with starting address after ssid
        }
        EEPROM.commit();
     }


    request->send(200, "text/html", "ESP Node MCU configured with value: " + inputMessage1 +inputMessage2+
                                     "<br><a href=\"/\">Return to Home Page</a>");
     
  });
  
  server.onNotFound(notFound);
  server.begin();
  Serial.println("HTTP server started");
delay(8000);
//Read data from the E2PROM

String readssid;
String readpw;

ssidLength=EEPROM.read(0);              //read back from e2prom
pwLength=EEPROM.read(1);

for(int i=0;i<ssidLength;i++)
{
  readssid=readssid+char(EEPROM.read(2+i));           // read ssid from eeprom
}
for(int i=0;i<pwLength;i++)
{
  readpw=readpw+char(EEPROM.read(2+ssidLength+i));    // read password from eeprom   addr: (2 header + ssidLength + i)
}

Serial.print("Connecting to ");
Serial.print(": ");
Serial.println(readssid);
WiFi.begin(readssid,readpw);
Serial.println("Please wait...");
delay(5000);// connect to wifi using ssid & password

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
  Serial.println("WL_NO_SSID_AVAIL.");
  ssid="NodeMCUReset";
  pw="12345678";
  goto flag;
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
