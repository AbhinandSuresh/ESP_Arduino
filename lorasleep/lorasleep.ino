#include <LowPower.h>
#include <SPI.h>
#include<String.h>
#include <LoRa.h>
#define echoPin 5
#define trigPin 6

long duration;
float level;
int flag=0;
int count =0;
char device_id[12] = "IoTuneZ Lora";

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  while (!Serial);  
  Serial.println("Ultrasonic Sensor HC-SR04  Sleep Activated");
  Serial.println("with Arduino Nano");
  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
void loop() {
  Serial.println("Going to sleep for 8 seconds");
  delay(100);
  //for(int i = 0; i<=1; i++)
  {
  LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,SPI_OFF, USART0_OFF, TWI_OFF);
  }
  delay(20);
  Serial.println("");
  Serial.println("Arduino: Hey I just Woke up");
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
  String LoRaMessage = String(device_id) + String(count)+">" + String(level);
  LoRa.beginPacket();  
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
  Serial.print("Sending Data over LoRa: ");
  Serial.println(LoRaMessage);
  count++;
  delay(50);
}
