#include <LowPower.h>
#include <SPI.h>
#include<String.h>
#include <LoRa.h>
#define echoPin 5
#define trigPin 6

byte localAddress = 0xFF;     // address of this device
byte destination = 0xBB;      // destination to send to
String acknowledgment;
int pos1, pos2;
long duration;
float level;
int flag=0;
byte count =0;
String device_id = "IoTuneZ Lora";
String Lora_Ra02;
int loraval;

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

  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(count);                    // add message ID
  LoRa.write(LoRaMessage.length());     // add payload length
  LoRa.print(LoRaMessage);              // add payload
  LoRa.endPacket();                     // finish packet and send it
  count++;                             // increment message ID
  Serial.println("Sending " +LoRaMessage + " to 0x" + (destination));
  
  String incoming = "";
 // onReceive(LoRa.parsePacket());
  //while (LoRa.available()) 
  {
   //incoming = LoRa.readString();;
  }
  //Serial.println(incoming);
  
  Serial.println("Going to sleep for 8 seconds");
  for(int i = 5; i<=1; i++)
  {
  LowPower.idle(SLEEP_1S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,SPI_OFF, USART0_OFF, TWI_OFF);
  }
}

/*void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return
  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming = LoRa.readString();;
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.println("Received from:0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println(incoming);
}*/
