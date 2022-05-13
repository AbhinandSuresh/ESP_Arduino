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
#include <LowPower.h>
#include <SPI.h>
#include<String.h>
#include <LoRa.h>
#define echoPin 5                //Waterproof ultrasonic level sensor pin to D5,D6 pins
#define trigPin 6
#define onAck "mon"
#define offAck "mof"
#define csPin = 10;              // LoRa radio chip select
#define resetPin = 9;            // LoRa radio reset
#define irqPin = 2;              // hardware interrupt pin
int loraval, sec = 1;            // initialize lora_received value variable and default sleep delay seconds
byte localAddress = 0xFF;        // address of this device
byte destination = 0x80;         // destination to send to
byte count = 0;
String acknowledgment, device_id = "IoTuneZ_906: ",pkt = "";
long duration;
float level;
bool isAcknoledged, isPktAvailable = false;

/*Function for arduino board to go to sleep for some seconds*/
void system_sleep(int sleepVal)
{
  delay(5);
  #ifdef DEBUG
  Serial.println("going to sleep for " + String(sleepVal) + "seconds.");
  Serial.flush();
  #endif
  for (sleepVal; (sleepVal) > 0; (sleepVal)--)
  {
    LowPower.idle(SLEEP_1S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
    delay(5);
    if (!LoRa.begin(433E6))
    {
      Serial.println("Starting LoRa failed!");
      while (1);
    }
  }
  return;
}

void LoRa_sendMessage(String message)
{
  LoRa_txMode();                        // set tx mode
  LoRa.beginPacket();                   // start packet
  LoRa.print(message);                  // add payload
  LoRa.endPacket(true);                 // finish packet and send it
}

void sendData()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(8);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the level
  level = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  //delay(100);
  String LoRaMessage = String(device_id) + String(count) + ">" + String(level);
  //delay(50);
  #ifdef DEBUG
  Serial.println("Sending " + LoRaMessage + " to 0x" + (destination));
  #endif
  LoRa_sendMessage(LoRaMessage);
  count++;  //increment message ID
}

void loraAck_wait()
{
  //Receive Ack
  //wait for Acknoledgement
  int count = 5000; //to wait for 5 seconds (5000 ms)
  while (!isAcknoledged && count--)
  {
    delay(1);
  }
  if (isAcknoledged)
  {
    #ifdef DEBUG
    Serial.println("Controller ACK");
    #endif
    isAcknoledged = false;
  }
  else
  {
    Serial.println("Controller NACK");
    sec = 15;
  }
}

void on_TxDone()
{
  #ifdef DEBUG
  Serial.println("TxDone");
  #endif
  LoRa_rxMode();
}

void on_receive(int packetSize)
{
  String incoming = "";
  while (LoRa.available())
  {
    incoming += (char)LoRa.read();
  }
  //delay(400);
  // if message is for this device, or broadcast, print details:
  #ifdef DEBUG
  Serial.print("Message: ");
  Serial.println(incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
  #endif
  isAcknoledged = true;
  if (incoming == onAck)
  {
    sec = 5; //set sleep time to 5 seconds
  }
  else if (incoming == offAck)
  {
    sec = 900; //set sleep time to 15 minutes
  }
  else
  {
    sec = 60; //go to sleep for one minute
  }
  LoRa_txMode();
}

void LoRa_rxMode() // lora receive mode
{
  LoRa.receive();
  #ifdef DEBUG
  Serial.println("rxMode");
  #endif
}

void LoRa_txMode() // lora transmit mode
{
  LoRa.idle();                          // set standby mode
  LoRa.disableInvertIQ();               //tx mode
  #ifdef DEBUG
  Serial.println("txMode");
  #endif
  delay(5);
}

void setup() // setup routine to run once
{
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  while (!Serial);
  #ifdef DEBUG
  Serial.println("IoTuneZ AWMS");
  Serial.println("Ultrasonic Sensor HC-SR04 Sleep Activated");
  Serial.println("Level Monitor Debugging Mode");
  #endif
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(433E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.onReceive(on_receive);
  LoRa.onTxDone(on_TxDone);
}

void loop() //loop routine
{
  LoRa.onReceive(on_receive);
  LoRa.onTxDone(on_TxDone);
  //LoRa_txMode();
  sendData(); //Read data from sensor and send it over LoRa
  loraAck_wait(); //Wait for ack for maximum 5 seconds
  //Go to Sleep as per the received acknowledgement
  system_sleep(sec);
}
