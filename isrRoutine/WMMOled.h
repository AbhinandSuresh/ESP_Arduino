/*This  library is  created   with necessary functions  which is utilized for initiation of the
   included OLED display on the Motor Controlling Module and also displaying needed informatoin to the user.
   Authuor:Peter Paul*/

#ifndef WMMOLED_H_
#define WMMOLED_H_

#include <Wire.h>
//For I2C Communication.
#include "SSD1306Wire.h"
//For initializing  the necessary information on the display

void oledInit(SSD1306Wire display);
//For initialization of the display :@object of SSD1306Wire

void oledClear(SSD1306Wire display);
#endif
