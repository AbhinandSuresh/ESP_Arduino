#include <ESP8266WebServer.h>
#include<ESP8266WiFi.h>

String ssid = "Redmi";
String pw="abhi1234";
int led= D0;
ESP8266WebServer server(80);

void setup() 
{
  pinMode(led,OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid,pw);
while(WiFi.status()!=WL_CONNECTED)
{
  yield();
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
  IPAddress ip =WiFi.localIP();
  Serial.println("Connected");
  Serial.print("IP Address is:");
  Serial.println(ip);

server.on("/",helloworld);
server.on("/ledon",ledon);
server.on("/ledoff",ledoff);
server.onNotFound(handleNotFound);
server.begin();
Serial.println("Server Started");
}

void handleNotFound()
{
  server.send(404,"text/plain", "page not found");
  }
void ledon()
{
  digitalWrite(led,LOW);
  Serial.println("LED IS TURNED ON");
  server.send(200, "text/html", "<html><body><H1>LED IS TURNED ON <br><br><br><a href =\"/ledon\"><button>LED ON</button></a> <br> <br><br> <a href =\"/ledoff\"><button>LED OFF</button></a>  </H1></body></html>");
  }

void ledoff()
{
  digitalWrite(led,HIGH);
  Serial.println("LED IS TURNED OFF");
  server.send(200, "text/html", "<html><body><H1>LED IS TURNED OFF <br> <br> <br><a href =\"/ledon\"><button>LED ON</button></a> <br><br><br>  <a href =\"/ledoff\"><button>LED OFF</button></a>  </H1></body></html>");
  }

void helloworld()
{

  server.send(200,"text/html","<HTML><BODY><H1>HELLO FROM ESP 8266 <br> <br> <br> <a href =\"/ledon\"><button>LED ON</button></a> <br> <br><br> <a href =\"/ledoff\"><button>LED OFF</button></a> </H1></BODy></HTML>");
  
  }

void loop() {
  server.handleClient();
  
}
