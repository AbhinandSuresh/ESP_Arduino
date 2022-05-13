/* 

The code is to set an interupt service routine (ISR) to turn on OLED display, to start pairing mode and do a soft reset.

Author: IoTunez Technologies, Kochi
*/

#include <Wire.h>
#include "SSD1306Wire.h"
#include "WMMOled.h"

#define SDA 4             //Data pin of OLED
#define SCL 5             //Clock pin
#define BAUD_RATE 9600    //Baud rate
#define ADDRESS 0x3c      //Address  for 128x96 OLED 

SSD1306Wire display(ADDRESS, SDA, SCL);

const int button = 2; //Digital pin to connect physical button on ESP8266
int lastButtonPressed = 0; //Counter to save the last state
int buttonPressed = 0; //Counter to set the current state
int timePressStart = 0;
int timePressStop = 0;
int PressedTime = 0;  
int count=0;


void ICACHE_RAM_ATTR isr() {             //ISR function for the button. Need to save it in RAM to prevent the code from crashing.

 int buttonState = digitalRead(button);  //Get whether the button is HIGH or LOW ( Pressed or Released)
 
 if (buttonState == HIGH && millis() > (timePressStop+200) ) 
 {  
  timePressStart = millis();
  buttonPressed = 1;

 }
 else
 {
  timePressStop = millis();
 }
}

//Function to return the value according to the total time the button is being pressed.

int getButtonPress(void)
{   
  int ret = 0;  //return value
  int timestamp = timePressStop < timePressStart ? millis() : timePressStop;
  int timediff = timestamp - timePressStart;
  if(!buttonPressed)
    return 0;
  if(lastButtonPressed <3 && timediff  > 6000)
  {
    ret  = 3;
  }
  else if(lastButtonPressed <2 && timediff  > 3000)
  {
    ret  = 2;
  }  
  else if(lastButtonPressed <1 && timediff  > 100) 
  {
    ret  = 1;
    count++;
  }
  if(ret)
  {
    lastButtonPressed = ret;
  }
  if (digitalRead(button) == LOW)
  {
    lastButtonPressed = 0;
    buttonPressed = 0;      
  }  
  return ret;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  pinMode(button , INPUT);
 attachInterrupt(digitalPinToInterrupt(button), isr, CHANGE);
 Wire.begin(SDA, SCL); //defining the custom pins for I2C interface

}

void loop() {
  
  switch(getButtonPress()){
    case 1:
    Serial.println("Pressed 1");
    if (count==1){ 
      oledInit(display);//intializing the dispaly:@Object of SSD1306Wire
      Serial.println("brr");
    }
    else if (count==2){
      Serial.println("boo");
      oledClear(display);
      //Time function display
    }
    else if(count==3){
      Serial.println("bee");
      //Waterlevel
    }
    else if(count==4){
      Serial.println("baa");
      count=0;
      //Usage
    }
    break;
    case 2:
    Serial.println("Pressed 2");
    break;
    case 3:
    Serial.println("Pressed 3");
    oledClear(display);
    ESP.restart();
    Serial.println("ERROR...");
    break;
    
  }
}
