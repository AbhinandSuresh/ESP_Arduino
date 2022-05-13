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
 * Pot
 *    1 to 5v
 *    2 to A0
 *    3 to gnd
*/

#include <SPI.h>
#include <LoRa.h> 
int pot = A0;
int preval=0;
 char device_id[12] = "Abhi's Lora";
void setup() {
  Serial.begin(9600);
  pinMode(pot,INPUT);
  
  while (!Serial);  
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop() {
  String LoRaMessage="";
  int val = map(analogRead(pot),0,1024,0,255);
  if (val==preval)
  {
  delay(1000);
  LoRaMessage = String(device_id) + "/" + String(val) + "&";
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  Serial.println(".");  
  LoRa.endPacket(); 
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
  }
}
