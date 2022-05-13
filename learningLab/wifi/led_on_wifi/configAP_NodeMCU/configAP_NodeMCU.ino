#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

String ssid = "NodeMcu";
String password = "pw123456";

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html","<HTML><body><H1>ESP8266 Configuration Mode</H1><br><H2>IoTuneZ Internship</H2><br><div><label for=\"ssid\">Enter New SSID :  </label><input type=\"text\" id=\"ssid\" name=\"ssid\"></div><br><div> <label for=\"pass\">Enter Password   :  </label><input type=\"password\" id=\"pass\" name=\"password\" minlength=\"8\" required></div><br> <a href =\"/config+ssid+&pass\"><button> SUBMIT </button></a> </body></HTML>");
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void handleNotFound()
{
  server.send(404,"text/plain", "ERROR 404: page not found");
  }


void loop() {
  server.handleClient();
}
