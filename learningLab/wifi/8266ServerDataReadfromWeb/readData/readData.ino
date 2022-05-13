#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
const char* ssid = "Redmi";
const char* password = "abhi1234";

const char* PARAM_INPUT_1 = "ssidNew";
const char* PARAM_INPUT_2 = "pwNew";

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
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

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
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
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
}

void loop() {
  
}
