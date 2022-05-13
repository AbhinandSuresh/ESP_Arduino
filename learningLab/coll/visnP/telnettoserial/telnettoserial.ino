#include <ESP8266WiFi.h>
#include <EEPROM.h>

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 2
WiFiServer server(23); // --> default port for communication usign TELNET protocol | Server Instance
WiFiClient serverClients[MAX_SRV_CLIENTS]; // --> Client Instanse
String ch;
String sub;
String SSIDs;
String Password;
unsigned long times=0;
unsigned long maxTime=10000;
String readSSID;
String readPassword;
String singlechar;
String delimiter=";";
String singlechar1;
void setup() {
Serial.begin(115200);
EEPROM.begin(512);
while(times<maxTime){///wait for 3 seconds to recieve any data from serial mointor
    times = millis();
    Serial.print(".");
    if(Serial.available()>0){
      ch=Serial.readString();
      delay(2000);
      break;
    }
    delay(1000);
}
sub=ch.substring(0,1);
//Serial.println(ch);
//Serial.println(sub);
sub.toLowerCase();
//Serial.println(sub);
if(sub=="c"){//if we recieve "c" charater enter into config mode
Serial.println("Entering configfuration mode.......... ");
Serial.println("Enter SSID and Password: ");//////here we ask to enter ssid and password with space in between 
while(!Serial.available());
String sub=Serial.readString();
int index=sub.indexOf(" ");
SSIDs= sub.substring(0,index);//seperate ssid and password and save it in eeprom
SSIDs= SSIDs + ";" ;
//Serial.println(SSIDs);
Password=sub.substring(index+1,sub.length());
Serial.println(Password);
for(int i=0;i<SSIDs.length();i++){//Write SSID and password to EEPROM
EEPROM.write(i,SSIDs[i]);
}
Password=Password+";";
for(int i=0;i<Password.length();i++){
EEPROM.write(0x0F+i,Password[i]);
Serial.println(Password[i]);
}
EEPROM.commit();
}
int i=0;
while(singlechar!=delimiter){//REading SSID and password from eeprom with character delimiter.
  readSSID=readSSID+char(EEPROM.read(i));
  singlechar=char(EEPROM.read(i+1));
  i++;
  }
i=0;
  while(singlechar1!=delimiter){
  readPassword=readPassword+char(EEPROM.read(0x0F+i));
  singlechar1=char(EEPROM.read(i+1));
  Serial.println(singlechar1);
  i++;
  }

Serial.begin(115200);
WiFi.begin(readSSID, readPassword);

// -->Try to connect to particular host for 20 times, If still not connected then automatically resets.
Serial.print("\nConnecting to "); Serial.println(readSSID);
int8_t j = 0;
while (WiFi.status() != WL_CONNECTED && j++ < 20) delay(500);
if(j == 21){
  Serial.print("Could not connect to"); Serial.println(readSSID);
  while(1);
}
//start UART and the server
Serial.begin(115200);
server.begin();
server.setNoDelay(true); // --> Won't be storing data into buffer and wait for the ack. rather send the next data and in case nack is received, it will resend the whole data

Serial.print("Ready! Use 'telnet ");
Serial.print(WiFi.localIP());
Serial.println(" 23' to connect");
}

void loop() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        Serial.print("New client: "); Serial.print(i);
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //check clients for data
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
        //get data from the telnet client and push it to the UART
        while(serverClients[i].available()) Serial.write(serverClients[i].read());
      }
    }
  }
  //check UART for data
  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }
}
