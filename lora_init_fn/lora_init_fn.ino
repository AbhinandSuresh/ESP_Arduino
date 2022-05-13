#include <LoRaInit.h>

void loraInit(const int cs,const int rst, const int irq,long freq)
{
  {
    LoRa.setPins(cs, rst, irq);
    if (!LoRa.begin(freq)) 
    {
     Serial.println("LoRa init failed. Check your connections.");
     while (true);                       // if failed, do nothing
    }
  LoRa.onReceive(onReceive);
  LoRa.onTxDone(onTxDone);
  }
  LoRa_rxMode();
}
