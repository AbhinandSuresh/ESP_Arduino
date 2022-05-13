#include<PubSubClient.h>
#include<WiFiClient.h>
#include<ESP8266WiFi.h>

#define SSID_ "Redmi"
#define PW "abhi1234"
#define SERVER "192.168.43.193"
#define PORT 1883

String ssid = SSID_;
String password = PW;

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message Received: on Topic [");
  Serial.print(topic);
  Serial.println("] : ");
  String msg = "";
  for (int i = 0; i < length; i++)
  {
    msg += (char)payload[i];
  }
  Serial.println(msg);
  msg.toLowerCase();
  if(msg=="on")
  {
    digitalWrite(LED_BUILTIN,LOW);
    }
      if(msg=="off")
  {
    digitalWrite(LED_BUILTIN,HIGH);
    }
}

WiFiClient wificlient;
PubSubClient client(SERVER, PORT, callback, wificlient);

void checkconnection () {

  while (!client.connected())
  {
    Serial.println("Connecting back to MQTT Broker");
    if (client.connect("arduinoClient")) {
      Serial.println("Connected Successfully");
      client.publish("RoomLight", "hello world");
      client.subscribe("led");  
    }
    else
    {
      Serial.println("MQTT Connection Failed");
      Serial.print(client.state());
      delay(1000);
      }
  }
}


void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN,OUTPUT);
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

void loop() {
  checkconnection();
  client.loop(); 
}
