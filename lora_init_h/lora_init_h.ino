#pragma once

void LoRa_rxMode() 
{
  LoRa.disableInvertIQ();               // normal mode
  LoRa.receive();                       // set receive mode
}

void LoRa_txMode() 
{
  LoRa.idle();                          // set standby mode
  LoRa.enableInvertIQ();                // active invert I and Q signalsLoRa.idle();
}
