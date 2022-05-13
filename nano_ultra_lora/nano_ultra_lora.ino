/*
 * IoTuneZ Technologies internship
 * Author : Abhinand Suresh
 * 
 * Connections:
 * LoRa
 *    Gnd,Gnd to gnd
 *    3v3 to 3v3
 *    RST to D9
 *    DIO0 to D2
 *    NSS to D10
 *    MOSI to D11
 *    MISO to D12
 *    SCK to D13   
 * Sensor
 *    Vcc to 5v
 *    Trigger to D6 
 *    Echo to D5
 *    GND to gnd
*/

#include <SPI.h>
#include <LoRa.h> 
#define echoPin 5 
#define trigPin 6

long duration;
float distance; 
float preval=0;
int flag=0;
int int_dis;
char device_id[12] = "IoTuneZ Lora";

void setup() 
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  while (!Serial);  
  Serial.println("Ultrasonic Sensor HC-SR04"); 
  Serial.println("with Arduino Nano");
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop() 
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(8);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  delay(200);
    String LoRaMessage = "dist = " + String(distance) + "&";
 
  Serial.println(LoRaMessage);
  /*
  String LoRaMessage="";
  int val = (int)distance;
  if (val==preval)
  {
    if (flag == 0)
    {
      delay(200);
      Serial.println("No Change in Measured Distance");  
      flag =1;
    }
  }
  else
  {
  LoRaMessage = String(device_id) + "/" + String(val) + "&";
  LoRa.beginPacket();  
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
  Serial.print("Sending Data over LoRa: ");
  Serial.println(LoRaMessage);
  preval= val; 
  delay(50);
  flag=0;
  }*/
}
