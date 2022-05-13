#include<Wire.h>


void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  

}

void loop() 
{
  int address,a;

  for(address=0;address<128;address++)
 {  
Wire.beginTransmission(address);
a=Wire.endTransmission();
if(a==0)
        {
  Serial.print("Device found at :");
  Serial.println(address);
   Serial.println("SCAN COMPLETE");
  delay(1000);
       }

 }

 

}
