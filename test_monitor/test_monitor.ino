#include <SPI.h>              // include libraries
#include <LoRa.h>
#include"msgType.h"
#define LORA_FREQUENCY 433E6  // Set LoRa frequency 
#define SERIAL_BAUD 9600      // Serial baud rate
#define CS_PIN 10             // LoRa radio chip select
#define RST_PIN 9             // LoRa radio Reset pin
#define IRQ_PIN 2  
unsigned char frameCount =0;
char genKey[16];
#define MAC_SIZE    6
#define MAX 256               //for random key generation

#define DEBUG
#define DEBUG_LEVEL 1
#ifdef DEBUG
#define debug_Println(level,msg) { \
        if(DEBUG_LEVEL >= level) \
        Serial.println(msg); \
        }
#define debug_Print(level,msg) { \
        if(DEBUG_LEVEL >= level) \
        Serial.print(msg); \
        }
#else
#define debug_Println(x)
#define debug_Print(x)
#endif

//uint8_t mac1 [6];
//uint8_t mac2 [6];
//uint8_t mac3 [6];
//uint8_t macpayload [3*MAC_SIZE];

//define Header and payload structures
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

void pairDevice(unsigned char authID)
{
    msgType messageType = JOIN_REQUEST;
    devType deviceType = MONITOR;
    Header header1;
    payload payload1;
    //LoRa.enableCrc();
    header1.authenticationId=authID;
    header1.deviceId[0]=0xaa;
    header1.deviceId[1]=0xab;
    header1.deviceId[2]=0xac;
    header1.deviceType = deviceType;
    payload1.messageType = messageType;
    payload1.utcTime=0;
    payload1.frameCount = frameCount;
    payload1.message1={'P'};
    header1.payloadLength=sizeof(payload1);
    unsigned char crc = 0x05;
    LoRa_txMode();
    LoRa.beginPacket();
    LoRa.write((uint8_t*)&header1, sizeof(Header));
    LoRa.write((uint8_t*)&payload1, sizeof(payload));
    //LoRa.enableCrc();
    LoRa.endPacket(); 
    onTxDone();
    debug_Println(1,"Message Sent! \nFrame "+ String(frameCount));
    debug_Println(1,"Dev Type, MessageType, Message, Frame Count: "+String(header1.deviceType)+" ,"+String(payload1.messageType)+" ,"+String(payload1.message1)+" ,"+String(payload1.frameCount));
    frameCount++;
    delay(10000);//FIX_ME
}

void genrandomKey(long x)
{
  int randNum =int(x);
  for (int i = 0; i < 16; i++)
    {
      randNum = random(0, 256); //funtion for obtaining random number between 0and 256
      genKey[i] = randNum;
    }
  debug_Println(1,"Key Generated...");  
  for (int i = 0; i < 16; i++)
  {
    debug_Print(1,genKey[i]);
  }
}


void LoRa_rxMode()
{
  LoRa.enableInvertIQ();                // active invert I and Q signals
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode()
{
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               // normal mode  
}

void onTxDone() {
  Serial.println("TxDone");
  LoRa_rxMode();
}

void onReceive(int packetSize) 
{//FIX_ME:
  String message = "";
  debug_Println(1,"\nMessage Received: ");
  while (LoRa.available()) 
  {
    message=LoRa.read();
    debug_Println(1,message);
  }
  
}

void loraInit()
{
  LoRa.setPins(CS_PIN, RST_PIN, IRQ_PIN);
  if (!LoRa.begin(LORA_FREQUENCY)) 
  {
    debug_Println(1,"\nLoRa init failed. Check your connections.");
    while (true);                       // if lora init failed, loop for ever.
  }

  debug_Println(1,"\nLoRa init succeeded.");
  debug_Println(1,"");
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
}

void setup() 
{
  Serial.begin(SERIAL_BAUD);                      //initialize Serial port communication for debugging
  debug_Println(1,"\nSerial Init Success...");    //display status for debugging
  loraInit();
}

void loop() 
{ loraInit();
  unsigned char authID=0xAD;
  pairDevice(authID);
  //genrandomKey(millis());
}
