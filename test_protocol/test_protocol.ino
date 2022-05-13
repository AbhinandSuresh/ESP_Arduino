#include <SPI.h>              // include libraries
#include <LoRa.h>
#include "msgType.h"
const long frequency = 433E6;  // LoRa Frequency
const int csPin = 10;          // LoRa radio chip select
const int resetPin = 9;        // LoRa radio reset
const int irqPin = 2;          // change for your board; must be a hardware interrupt pin
unsigned char frameCount =0;
static unsigned long previousMillis = 0;

struct Header{
  unsigned char authenticationId;
  unsigned char deviceId[3];
  unsigned char deviceType;
  unsigned char payloadLength;
  };
  
struct payload{
  unsigned char messageType;
  unsigned long utcTime;
  unsigned char frameCount;
  unsigned char message1;
  };
  

void lora_init()
{
    LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) 
  {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
}
void setup() {
  Serial.begin(9600);                   // initialize serial
  while (!Serial);

  LoRa.setPins(csPin, resetPin, irqPin);

  if (!LoRa.begin(frequency)) 
  {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
  Serial.println();
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  //LoRa_rxMode();
   //LoRa_txMode();
}

void loop() {
  //if (runEvery(2000)) { // repeat every 5 sec
  delay (2000);
  {
    lora_init();
    msgType messageType = JOIN_REQUEST;
    devType deviceType = MONITOR;
    Header header1;
    header1.authenticationId=0xaa;
    header1.deviceId[0]=0xAB;
    header1.deviceId[1]=0xAC;
    header1.deviceId[2]=0xAD;
    header1.deviceType=deviceType;
    payload payload1;
    payload1.messageType=messageType;
    payload1.utcTime=3122151;
    payload1.frameCount = frameCount;
    payload1.message1={'P'};
    header1.payloadLength=sizeof(payload1);
     Serial.println("ayyo amme");
    LoRa_txMode();
    Serial.println("havoo");
    unsigned char crc = 1;
    /*LoRa.beginPacket();                   // start packet
    LoRa.print(header1);              
    LoRa.print(payload1);
    Lora.print(crc);
    LoRa.endPacket(true);                 // finish packet and send it*/
    //LoRa_sendMessage("br.....");
    LoRa.beginPacket();
    LoRa.write((uint8_t*)&header1, sizeof(Header));
   // LoRa.write((uint8_t*) &payload1.messageType, sizeof(payload1.messageType));
   Serial.println(",,,");
    LoRa.endPacket(); 
    frameCount++;
    Serial.println("Message Sent! \nFrame "+ String(frameCount));
  }
}

void LoRa_rxMode(){
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode(){
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode  
}

void LoRa_sendMessage(String message) {
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void onReceive(int packetSize) {
  String message = "";

  while (LoRa.available()) {
    message += (char)LoRa.read();
  }
  Serial.print("Node Receive: ");
  Serial.println(message);
}

void onTxDone() {
  Serial.println("TxDone");
  LoRa_rxMode();
}

boolean runEvery(unsigned long interval)
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}
