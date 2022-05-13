/*
   Automatic Water Management system(AWMS)
   Project done at IoTuneZ technologies
   as part of industrial internship
   Author: Mr.Greatin Baby and Mr. Abhinand Suresh
   Code for water level monitoring module
   LowPower library is used for putting arduino into sleep mode
   Communication is done using radio wave transmission of packets.
   LoRa modules are used for transmitting datas.
*/
#include "msgType.h"
#include<LoRa.h>
#include <LowPower.h>
#include <SPI.h>
#include <LoRa.h>
#include <String.h>
#define ECHO_PIN 5            // Echo pin of Ultrasonic sensor
#define TRIGGER_PIN 6         // Trigger pin of Ultrasonic sensor
#define LORA_FREQUENCY 433E6  // Set LoRa frequency 
#define BAUD_RATE 9600        // Serial baud rate
#define ON_ACK_SLEEP 5        // Time in seconds for Board sleeps while motor is on
#define OFF_ACK_SLEEP 900     // Time in seconds for Board sleeps while motor is off
#define NO_ACK_SLEEP 60       // Time in seconds for Board sleeps while NACK state
#define ACK_WAIT_COUNT 5000   // Time in milliseconds to wait for Acknowledgement (5000 ms default)
#define CS_PIN 10             // LoRa radio chip select
#define RST_PIN 9             // LoRa radio Reset pin
#define IRQ_PIN 2             // LoRa hardware interrupt pin
#define onAck 0x01            // Expected Acknowledgement for motor power on (May be updated after cyphering)
#define offAck 0x02           // Expected Acknowledgement for motor power off (May be updated after cyphering)
#define authenticationId 0xad;            //Fancy ID for authentication purpose. To understand IoTuneZ devices
/*
 * Define DEBUG for Debug Mode
 * DEBUG_LEVEL 0,1,2 can be used for various debugging levels
 * 0: End user mode
 * 1: Debug mode
 * 2: Verbose Mode 
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
#else
#define debug_Println(x)
#define debug_Print(x)
#endif

/* States defined 
 * value of Motor Status : 
   on:      1(0x01)
   off      2(0x02)
   error    3(0x03)
   dry run  4(0x04)
 * The value of Device: 
   Level Monitor: 1 (0x01) 
   Conroller:     2 (0x02)
   Relay Board:   3 (0x03)
 * The default value of machine state : 
   Pairing Mode:  1   (0x0a) 
   Config Mode:   2   (0x0b) 
   Receive Mode:  3   (0x10)
   Transmit Mode: 4   (0x11)
*/
enum motor_state
{
  motor_on=0x01,      //Motor State representing motor is ON
  motor_off=0x02,     //Motor State representing motor is OFF
  motor_error=0x03,   //Motor State representing motor is in ERROR State 
  motor_dryRun=0x04   //Motor State representing motor is in Dry run State
};
enum device
{
  level_monitor=0x01,
  controller,
  relay_module
};
enum machine_state
{
  pairing=0x01,          // Machine in Pairing mode
  configuring,           // Machine in Configuration mode
  monitoring,
  motorOn,
  noAck,
  error 
};
enum message_type
{
  joinRequest=0x01,
  joinAccept,
  configMsg,
  level_monitoringMessage,
  ack_msgRx
  ack_motorOn,
  ack_motorOff
  dry_runMsg,
};

void pairMe() 
{
  enum msgType messageType;
  unsigned long timeUTC=0;
  unsigned char frameCount=1;
  unsigned char broadcastAddress=0xFF;
  unsigned char authID = authenticationId;
  unsigned char pairMsg=0x01;             // for "pair me" message
  joinRequest joinRequest;                // initialize the struct joinRequest of datatype joinRequest as defined in header msgType.h
  joinRequest.devType= MONITOR;
  messageType = JOIN_REQUEST;
  LoRa.beginPacket();
  LoRa.print(authID);
  LoRa.print(broadcastAddress);
  LoRa.print(joinRequest.devType);
  LoRa.print(messageType);
  LoRa.print(timeUTC);
  LoRa.print(frameCount);
  LoRa.print(pairMsg);
  LoRa.endPacket();
}

int sec = 1;              //default sleep delay seconds
byte localAddress = 0xFF; // address of this device
byte destination = 0x80;  // destination to send to ; will be updated after configuration
String acknowledgment;
long duration;
float level;
byte count = 0;
int loraval;
bool isPktAvailable = false;
bool isAcknoledged;
struct  pkt {
  int device_id;
  };
  
void loraSetup()
{
  if (!LoRa.begin(LORA_FREQUENCY))
    {
      debug_Println(0,"Starting LoRa failed!");
      while (1);
    }
}
/*Function for arduino board to go to sleep for some seconds*/
void system_sleep(int sleepVal)
{
  debug_Println(2,"going to sleep for " + String(sleepVal) + "seconds.");
  Serial.flush();
  for (sleepVal; (sleepVal) > 0; (sleepVal)--)
  {
    LowPower.idle(SLEEP_1S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
  }
  return;
}

float water_levelSense()
{
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigger Pin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(8);
  digitalWrite(TRIGGER_PIN, LOW);
  // Reads the echo Pin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the level
  level = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  return level;
}

void LoRa_sendMessage(String message)
{
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void sendData(float message)
{
  String LoRaMessage = String(level_monitor) + String(count) + ">" + String(message);
  debug_Println(2,"Sending " + LoRaMessage + " to 0x" + (destination));
  LoRa_sendMessage(LoRaMessage);
  count++;  //increment message ID
}

void loraAck_wait()
{
  int count= ACK_WAIT_COUNT;
  //Receive Ack
  //wait for Acknoledgement
  while (!isAcknoledged && count--)
  {
    delay(1);
  }
  if (isAcknoledged)
  {
    debug_Println(2,"Controller ACK");
    isAcknoledged = false;
  }
  else
  {
    debug_Println(0,"Controller NACK");
    sec = NO_ACK_SLEEP;
  }
}

void on_TxDone()
{
  debug_Println(2,"TxDone");
  LoRa_rxMode();
}

void on_receive(int packetSize)
{
  String incoming = "";
  while (LoRa.available())
  {
    incoming += (char)LoRa.read();
  }
  // if message is for this device, or broadcast, print details:
  debug_Print(2,"Message: ");
  debug_Println(2,incoming);
  debug_Println(2,"RSSI: " + String(LoRa.packetRssi()));
  debug_Println(2,"Snr: " + String(LoRa.packetSnr()));
  isAcknoledged = true;
  //isPktAvailable = true;
  if (incoming == onAck)
  {
    sec = ON_ACK_SLEEP;
  }
  else if (incoming == offAck)
  {
    sec = OFF_ACK_SLEEP;
  }
  else
  {
    sec = NO_ACK_SLEEP;
  }
  LoRa_txMode();
}

void LoRa_rxMode()
{
  LoRa.receive();
  debug_Println(1,"rxMode");
}

void LoRa_txMode()
{
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               //tx mode
  debug_Println(1,"txMode");
}

void setup()
{
  Serial.begin(BAUD_RATE);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  while (!Serial);
  debug_Println(2,"Ultrasonic Sensor HC-SR04 Sleep Activated");
  debug_Println(2,"with Arduino Nano");
  debug_Println(2,"LoRa Sender");
  LoRa.setPins(CS_PIN, RST_PIN, IRQ_PIN);
}

void loop()
{
  loraSetup(); // initialize LoRa
  LoRa.onReceive(on_receive);
  LoRa.onTxDone(on_TxDone);
  pairMe();
  float val= water_levelSense(); //Read distance from the sensor to the water level
  sendData(val); //Send data over LoRa
  loraAck_wait(); //Wait for ack for maximum 5 seconds
  //Go to Sleep as per ack
  system_sleep(sec);
}
