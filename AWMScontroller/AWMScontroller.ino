#include <SPI.h>              // include libraries
#include <LoRa.h>
#define HIGH_THRESHOLD 130
#define LOW_THRESHOLD 20
#define onAck "mon"            // Expected Acknowledgement for motor power on (May be updated after cyphering)
#define offAck "mof"           // Expected Acknowledgement for motor power off (May be updated after cyphering)
#define LORA_FREQUENCY 433E6  // Set LoRa frequency 
#define BAUD_RATE 9600        // Serial baud rate

const int csPin = 15;           // LoRa radio chip select
const int resetPin = 16;        // LoRa radio reset
const int irqPin = 4;           // change for your board; must be a hardware interrupt pin
int loraval;
bool isPktAvailable = false;
bool isTxdone = true;
unsigned char localAddress = 0xff;     // address of this device
unsigned char destination = 0x80;      // destination to send to
byte msgCount = 0;
String message;
String device_id;
String Lora_Ra02;
int pos1, pos2;
String incoming;
#define MOTOR D1

void setup() {
  Serial.begin(BAUD_RATE);                   // initialize serial
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");
  Serial.println();
  Serial.println("LoRa Simple Gateway");
  Serial.println("Only receive messages from nodes");
  Serial.println("Tx: invertIQ enable");
  Serial.println("Rx: invertIQ disable");
  Serial.println();

  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  LoRa_rxMode();
}

void LoRa_rxMode() 
{
  LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
  //delay(10);
}

void LoRa_txMode() 
{
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();                // active invert I and Q signalsLoRa.idle();
  //delay(10);
}

void txloop()
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
}
void LoRa_sendMessage(String LoRaMessage)
{   int count;
    LoRa_txMode();                        // set tx mode
    LoRa.beginPacket();                   // start packet
    LoRa.print(LoRaMessage);              // add payload
    LoRa.endPacket(true);                 // finish packet and send it
    LoRa_rxMode(); 
}

void onReceive(int packetSize) 
{
  if (packetSize == 0) return;          // if there's no packet, return
  String incoming = "";
  while (LoRa.available()) 
  {
    incoming = LoRa.readString();
  }
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();
  pos1 = incoming.indexOf('>');
  pos2 = incoming.indexOf('&');
  device_id = incoming.substring(0, pos1);
  Lora_Ra02 = incoming.substring(pos1 + 1, pos2);
  loraval = Lora_Ra02.toInt();
  Serial.println(loraval);
  isPktAvailable = true;
}

void onTxDone() 
{
  Serial.println("TxDone");
  isTxdone = true;
  LoRa_rxMode();
}

void loop() 
{
  txloop();
}
