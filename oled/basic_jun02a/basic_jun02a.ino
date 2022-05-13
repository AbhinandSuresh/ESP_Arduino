#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

String buff, newbuff;
int a,b,i; 

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(300);

  testscrolltext();    // Draw scrolling text

  test();

}

void loop() {
  
  while (!Serial.available());
  test();
}

void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(1.5); // 
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("IoTuneZ"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x28);
  delay(1000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x28);
  delay(1000);
  display.stopscroll();
  delay(1000);
}

void test() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("IoTuneZ Technologies"));     
  display.println(F("Internship"));
  display.println(F(" ")); 
  display.println(F(" BMI OVER SERIAL :")); 
     display.println(F(" "));
     display.println(F(" ARDUINO TRAINING "));
     display.display();
      // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
}
