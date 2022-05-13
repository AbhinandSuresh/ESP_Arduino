/*
   Automatic Water Management system(AWMS)
   Project done at IoTuneZ technologies
   as part of industrial internship
   Author: R&D lead and Interns at IoTuneZ Technologies
   Code for Controller
   Communication is done using radio wave transmission of packets.
   LoRa modules are used for transmitting datas.
*/
#include <SPI.h>              // include libraries
#include <LoRa.h>
#define LORA_FREQUENCY 433E6  // Set LoRa frequency
#define SERIAL_BAUD 9600      // Set serial baud rate for debugging
/*
 * Define DEBUG for Debug Mode
 * DEBUG_LEVEL 0,1,2 can be used for various debugging levels
 * 0: Verbose mode
 * 1: Debug mode
 * 2: End user mode
*/
#define DEBUG
#define DEBUG_LEVEL 1
#ifdef DEBUG
#define debug_Println(level,msg) { \
        if(DEBUG_LEVEL <= level) \
        Serial.println(msg); \
        }
#define debug_Print(level,msg) { \
        if(DEBUG_LEVEL <= level) \
        Serial.print(msg); \
        }
#else
#define debug_Println(x)
#define debug_Print(x)
#endif

const int csPin = 15;           // LoRa radio chip select
const int resetPin = 16;        // LoRa radio reset
const int irqPin = 4;           // change for your board; must be a hardware interrupt pin

//function to initialize LoRa
void loraInit()
{
  LoRa.setPins(csPin, resetPin, irqPin);
  if (!LoRa.begin(LORA_FREQUENCY)) 
  {
   debug_Println(1,"\nStarting LoRa failed..! Error.");
   while (true);                     // if failed, halt.
  }
}

//function to initilize serial communication for debugging
void serialInit()
{
  Serial.begin(SERIAL_BAUD);
}
void setup() 
{
  /*
   * add setup and initialization routine(s) here
  */
  //Initialize Lora and halt if any failure.
  serialInit();
  debug_Println(1,"\nSerial Init Success...");
  loraInit();
  debug_Println(1,"Lora Init Success...");
  /*
   * add setup and initialization routine(s) here
  */
}

void loop() 
{
  /*
   * add main thread routine(s) here
  */
  debug_Println(2,"Main thread running...");
  delay(1000); //FIXME: REMOVE THIS LINE AFTER ADDING MAIN THREAD FUNCTIONS
  /*
   * add main thread routine(s) here
  */
}
