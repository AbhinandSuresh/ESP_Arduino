#include <dht11.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define DHT11PIN 5
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define pin 14 
dht11 DHT11;
int humi = 0;
int temp = 0;
void setup()
{
    Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) 
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(300);
 
}

void loop()
{
  //read temperature humidity data

  int j;
  unsigned int loopCnt;
  int chr[40] = {0};
  unsigned long time1;
bgn:
  delay(2000);
  //Set interface mode 2 to: output
  //Output low level 20ms (>18ms)
  //Output high level 40μs
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delay(20);
  digitalWrite(pin, HIGH);
  delayMicroseconds(40);
  digitalWrite(pin, LOW);
  //Set interface mode 2: input
  pinMode(pin, INPUT);
  //High level response signal
  loopCnt = 10000;
  while (digitalRead(pin) != HIGH){
    if (loopCnt-- == 0){
      //If don't return to high level for a long time, output a prompt and start over
      Serial.println("HIGH");
      goto bgn;
    }
  }
  //Low level response signal
  loopCnt = 3000;
  while (digitalRead(pin) != LOW){
    if (loopCnt-- == 0){
      //If don't return low for a long time, output a prompt and start over
      Serial.println("LOW");
      goto bgn;
    }
  }
  //Start reading the value of bit1-40
  for (int i = 0; i < 40; i++){
    while (digitalRead(pin) == LOW){}
    //When the high level occurs, write down the time "time"
    time1 = micros();
    while (digitalRead(pin) == HIGH){}
    //When there is a low level, write down the time and subtract the time just saved
    //If the value obtained is greater than 50μs, it is ‘1’, otherwise it is ‘0’
    //And save it in an array
    if (micros() - time1  > 50){
      chr[i] = 1;
    } else {
      chr[i] = 0;
    }
  }

  //Humidity, 8-bit bit, converted to a value
  humi = chr[0] * 128 + chr[1] * 64 + chr[2] * 32 + chr[3] * 16 + chr[4] * 8 + chr[5] * 4 + chr[6] * 2 + chr[7];
  //Temperature, 8-bit bit, converted to a value
  temp = chr[16] * 128 + chr[17] * 64 + chr[18] * 32 + chr[19] * 16 + chr[20] * 8 + chr[21] * 4 + chr[22] * 2 + chr[23];

    Serial.print("temp:");
    Serial.print(temp);
    Serial.print("    humi:");
    Serial.println(humi);
    
     display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("IoTuneZ Technologies"));     
  display.println(F("Internship"));
  display.println(F("   ABHINAND SURESH ")); 
     display.println(F(" "));
     display.println(F("Temperature in *C: "));
      display.println(temp);
      display.println(F("Humidity in %: "));
      display.println(humi);
     display.display();
// Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(2500);
  display.invertDisplay(false);
  delay(2500);
}
