#include <SPI.h>
#include <LoRa.h>

#define ss D11
#define rst D9
#define dio0 D2
//#define motor D1

byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to

//String device_id;
//String Lora_Ra02;
//int pos1, pos2;

void setup() {
  //pinMode(motor, OUTPUT);
  Serial.begin(9600);
  //digitalWrite(motor, HIGH); //initial-> OFF
 // pinMode(LED_BUILTIN,OUTPUT);
 // digitalWrite(LED_BUILTIN, HIGH);
  /* FIX IT: maybe used for debuging later
    Serial.begin(9600);
    while (!Serial);
    Serial.println(" ");
    Serial.println("LoRa Receiver Callback");*/
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    //    Serial.println("Starting LoRa failed!");
    while (1);
  }
  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  /*int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    String LoRaData = LoRa.readString();

    //    while (LoRa.available()) {
    //    LoRa.read();
    //}

    pos1 = LoRaData.indexOf('>');
    pos2 = LoRaData.indexOf('&');

    device_id = LoRaData.substring(0, pos1);
    Lora_Ra02 = LoRaData.substring(pos1 + 1, pos2);
  }

  //Compare received value to turn off or on the motor/LED
  int loraval = Lora_Ra02.toInt();
  if (loraval > 130)
  {
    Serial.println(loraval);
    digitalWrite(motor, LOW);//ON
    digitalWrite(LED_BUILTIN, LOW);
  }
  else if (loraval < 21)
  {
    Serial.println(loraval);
    digitalWrite(motor, HIGH);//OFF
   digitalWrite(LED_BUILTIN, HIGH);
  }*/
    onReceive(LoRa.parsePacket());
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
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
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
}
