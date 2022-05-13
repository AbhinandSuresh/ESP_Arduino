/* 
 *  AWMS Motor controller program.
 *  Authors: R&D Head and Interns at IoTuneZ Technologies
 */
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include"msgType.h"
#define LORA_FREQUENCY 433E6  // Set LoRa frequency
#define SERIAL_BAUD 9600      // Set serial baud rate for debugging
/*
 * Define DEBUG for Debug Mode
 * DEBUG_LEVEL 0,1,2 can be used for various debugging levels
 * 0: Verbose mode
 * 1: Debug mode
 * 2: End user mode
*/
#define DEBUG
#define DEBUG_LEVEL 1
#ifdef DEBUG
#define debug_Println(level,msg) { \
        if(DEBUG_LEVEL <= level) \
        Serial.println(msg); \
        }
#define debug_Print(level,msg) { \
        if(DEBUG_LEVEL <= level) \
        Serial.print(msg); \
        }
#define debug_Print(level,msg,type) { \
        if(DEBUG_LEVEL >= level) \
        Serial.print(msg,type); \
        }
#else
#define debug_Println(x)
#define debug_Print(x)
#endif

#define CS_PIN 15              // LoRa radio chip select
#define RST_PIN 16             // LoRa radio Reset pin
#define IRQ_PIN 4              // LoRa hardware interrupt pin

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
bool isPktAvailable = false;
bool isTxdone = true;
bool sendAck=0; 
Header header1;
payload payload1;

void onReceive(int packetSize) 
{
  if (packetSize == 0) return;          // if there's no packet, return
  int i=0;
  //int packetsize = LoRa.parsePacket();
  char incomingMsg[sizeof(Header)+sizeof(payload)]={};
  while (LoRa.available())
{
        //char incoming =(char)LoRa.read();
        incomingMsg[i]=(char)LoRa.read();
        i++;
        sendAck=1; 
}
        for(i=0;i<(sizeof(Header)+sizeof(payload));i++)
        {
          debug_Print(1,incomingMsg[i],HEX);
          }
      header1.authenticationId=incomingMsg[0];
      header1.deviceId[0]=incomingMsg[1];
      header1.deviceId[1]=incomingMsg[2];
      header1.deviceId[2]=incomingMsg[3];
      header1.deviceType=incomingMsg[4];
      header1.payloadLength=incomingMsg[5];
      payload1.messageType=incomingMsg[6];
      payload1.utcTime=incomingMsg[7];
      payload1.frameCount=incomingMsg[11];;
      payload1.message1=incomingMsg[12];     
//}
  debug_Println(1,"\n*******************\nMessage Received.\n______________________________ ")
  debug_Println(1,"Auth ID: ");
  debug_Print(1,header1.authenticationId,HEX);
  debug_Println(1,"\nDev ID: ");
  debug_Print(1,header1.deviceId[0],HEX);
  debug_Print(1,header1.deviceId[1],HEX);
  debug_Print(1,header1.deviceId[2],HEX);
  debug_Println(1,"\nDev Type: ");
  debug_Println(1,header1.deviceType);
  debug_Println(1,"Frame Count: ");
  debug_Println(1,payload1.frameCount);
  debug_Println(1,"Message: ");
  debug_Println(1,char(payload1.message1));
  int rSsi = LoRa.rssi();
  genrandomKey(rSsi);
  genrandomID(rSsi);
}

//Program to initialize LoRa
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
}



void genrandomID(int x)
{
  int randNum =x;
  char genKey[3];
  for (int i = 0; i < 16; i++)
    {
      randNum = random(0, 256); //funtion for obtaining random number between 0and 256
      genKey[i] = randNum;
    }
  debug_Println(1,"\nDevice ID Generated...");  
  for (int i = 0; i < 3; i++)
  {
    debug_Print(1,genKey[i],HEX);
  }
}

void genrandomKey(int x)
{
  int randNum =x;
  char genKey[16];
  for (int i = 0; i < 16; i++)
    {
      randNum = random(0, 256); //funtion for obtaining random number between 0and 256
      genKey[i] = randNum;
    }
  debug_Println(1,"\nKey Generated...");  
  for (int i = 0; i < 16; i++)
  {
    debug_Print(1,genKey[i],HEX);
  }
}

void serialInit()
{
  Serial.begin(SERIAL_BAUD);
}

void LoRa_rxMode()
{
  LoRa.disableInvertIQ();                // normal mode
  LoRa.receive();                        // set receive mode
}

void LoRa_txMode()
{
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();               // idle mode  
}

void onTxDone() {
  Serial.println("TxDone");
  isTxdone = true;
   LoRa_rxMode();
}

 /*void txloop()
{
  if (isTxdone)
  {
    LoRa_rxMode();
    isTxdone = false;
  }

  if (!isPktAvailable)
    return;


  isPktAvailable = false;
  delay(500);
 //LoRa_sendMessage("HELLO");
  if (loraval > HIGH_THRESHOLD)
  {
    digitalWrite(MOTOR, LOW);//ON
    digitalWrite(LED_BUILTIN, LOW);
    delay(700);//FIXME
    LoRa_sendMessage(onAck);
    Serial.println("Sending ON");
  }
  else if (loraval < LOW_THRESHOLD)
  {
    digitalWrite(MOTOR, HIGH); //OFF
    digitalWrite(LED_BUILTIN, HIGH);
    delay(700); //FIXME: Depending on the receive window time
    LoRa_sendMessage(offAck);
    Serial.println("Sending OFF");
  }  
})*/

void pairAck(unsigned char id1,unsigned char id2,unsigned char id3,unsigned char frameCount)
{
    delay(500);
    msgType messageType = JOIN_ACCEPT;
    devType deviceType = CONTROLLER;
    Header header2;
    payload payload2;
    header1.authenticationId=0xAD;
    header2.deviceId[0]=id1;
    header2.deviceId[1]=id2;
    header2.deviceId[2]=id3;
    header2.deviceType = deviceType;
    payload2.messageType = messageType;
    payload2.utcTime=0;
    payload2.frameCount = frameCount;
    response ackSuccess = SUCCESS;
    payload2.message1=ackSuccess;
    header2.payloadLength=sizeof(payload2);
    unsigned char crc = 0x05;
    delay(400);
    LoRa_txMode();
    LoRa.beginPacket();
    LoRa.print("HELLO");
    //LoRa.write((uint8_t*)&header2, sizeof(Header));
    //LoRa.write((uint8_t*)&payload2, sizeof(payload));
    LoRa.endPacket(true); 
    debug_Println(1,"\nack ayachu")
    onTxDone();
    }

void loraLoop()
{
  if(sendAck==1)
  {
    delay(700);
      debug_Println(1,"\nsendAck set aayi");
      pairAck(header1.deviceId[0],header1.deviceId[1],header1.deviceId[2],payload1.frameCount);
      sendAck=0;
  }
  if (isTxdone)
  {
    LoRa_rxMode();
    isTxdone = false;
  }
  if (!isPktAvailable)
    return; 
       
  isPktAvailable = false;
  //delay(500);
  debug_Println(1,"\n#############################\nkastam thanne mothalaaleee.............");
}

void setup() 
{
  //Initialize Lora and halt if any failure.
  serialInit();
  debug_Println(1,"\nSerial Init Success...");
  loraInit();
  //LoRa.receive();  
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();
  debug_Println(1,"Lora Init Success...");
  // add setup and initialization routines here
}

void loop() 
{
  loraLoop();
}
