#include <LowPower.h>
#include <SPI.h>
#include<String.h>
#include <LoRa.h>
#define echoPin 5
#define trigPin 6
/*
 * program to Send water level from a tank to controller near the motor/pump
 * Author: Abhinand Suresh
 * Created during IoTunenZ internship
*/

long duration;
int flag=0;
char ID[12] = "IoTuneZ Lora";

void setup() 
{
  //setup code, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 
  Serial.begin(9600);
  if (!LoRa.begin(433E6))
  {
    Serial.println("Lora :(");
    while (1);
  }
  LoRa.receive();
}

void sleepMin(int minutes)
  {
    /*
     * Program for microcontroller to sleep for some minutes
    */
    int sleepTime= (minutes*60)/8;
  for(int i = 0; i<=sleepTime; i++)
    {
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,SPI_OFF, USART0_OFF, TWI_OFF);
    }
  delay(20);
  }
void sleepSec(int sleepTime)
  {
    /*
     * Program for microcontroller to sleep for some seconds
    */
  for(int i = 0; i<=sleepTime; i++)
    {
    LowPower.idle(SLEEP_1S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,SPI_OFF, USART0_OFF, TWI_OFF);
    }
  delay(20);
  }

float readSensor()
  {
    /*
     * Program for reading sensor datas
    */
    float level;
    // Clears the trigPin condition
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
    delay(200);
    return level;
  }
String append(String ID,float tankLevel,float batteryLevel)
{
  /*
    * Program for appending datas to a string
   */
  String LoRaMessage = String(ID) + ">" + String(tankLevel) + "&" + String(batteryLevel);
  return LoRaMessage;
}
void transmit(String Txdata)
{
  /*
      * Program for sending datas over radio
   */
  LoRa.beginPacket();  
  LoRa.print(Txdata);
  LoRa.endPacket();
  Serial.println(Txdata);
}

String receiveAck()
{
  String LoRaData = LoRa.readString();
  //Serial.println("Data :");
   Serial.print(LoRaData);
  //while (LoRa.available()) {
  //String LoRaData = LoRa.readString();
 // Serial.println("Data :");
  //Serial.print(LoRaData);
    return LoRaData;
}

void loop() 
{
  // put your main code here, to run repeatedly:
  float level= readSensor();
  String loramsg= append(ID,level,NULL);
  transmit(loramsg);
  sleepSec(12);
  /*String ack=receiveAck();
  Serial.println(ack);
  int count=0;
  if(ack == "mon")
  {
    sleepSec(5);
  }
  else if(ack=="mof"||ack=="idl")
  {
    sleepSec(10);
  }
  else
  {
    for(count;count<2;count++)
    {
      transmit(loramsg);
      delay(20);
      }
  }*/
}
