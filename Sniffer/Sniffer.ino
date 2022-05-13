#include <SPI.h>
#include <LoRa.h>
/*#define ss 10
  #define rst 9
  #define dio0 2*/ //for uno

#define ss 15
#define rst 16
#define dio0 4 //for esp8266

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  String  pkt = "";
  String pktstr = "";
  if (packetSize && packetSize < 128) {
    // received a packet


    int total = 0;
    // read packet
    int i = 0;
    if (packetSize < 100) // filtering out garbage
      while (LoRa.available()) {
        pkt += (char)LoRa.read();
        total ++;
        if (total > packetSize)
        {
          break;
        }
      }
    Serial.println (pkt);
    // print RSSI of packet
    if (total == packetSize)
    {
      for (int j = 0; j < total; j++)
      {
        i++;
        if ((unsigned char)pkt[j] < 0x10)
        {
          Serial.print ("0");
        }
        Serial.print((unsigned char)pkt[j], HEX);
        Serial.print (" ");
        if (pkt[j] > 0x19 && pkt[j] < 127 )
        {
          pktstr += (char)pkt[j];
        }
        else
        {
          pktstr += '?';
        }
        if (i >= 16)
        {
          i = 0;
          Serial.print (" : ");
          Serial.println(pktstr);
          pktstr = "";
        }
      }
      while (16 - i++)
      {
        Serial.print("   ");
      }
      Serial.print (" : ");
      Serial.println(pktstr);
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
      Serial.print("Received packet of size : " );
      Serial.println( packetSize);
      Serial.println("------------------------------------------------------------------\n\n");
    }
  }
}
