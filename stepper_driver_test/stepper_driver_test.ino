#define pulseDelay 5000   // time_in milli/microseconds for pulse width on and off

int stepint; 
int freePin=9, dirPin=10,stepPin=11;
int errNeg=7, errPos=8;
int com=2;

void setup()   
{  
    pinMode(stepPin,OUTPUT);
    pinMode(errNeg,OUTPUT);
    pinMode(errPos,OUTPUT);
    pinMode(dirPin,OUTPUT);
    pinMode(freePin,OUTPUT);
    pinMode(com,OUTPUT);
    digitalWrite(errNeg,1);
    digitalWrite(errPos,LOW);
    digitalWrite(stepPin,0);
    digitalWrite(dirPin,0);
    digitalWrite(freePin,LOW);
    digitalWrite(com,LOW);
    Serial.begin(9600);  
    Serial.println("Enter Step Count:");  
    
}  
  
void loop()   
{  
    
    while (Serial.available()==0){}             // wait for user input via serial/com port
    stepint = Serial.parseInt();                //Read user serial data input and hold it in a variable
    while(stepint!=0){
    Serial.println("-------------------------"); //Showing the details in serial monitor  
    Serial.println("Steps: ");
    Serial.print(stepint);
    triggerPulse(stepint);                      //trigger stepint count of digital pulses and set 
    stepint=0;
    Serial.flush();
    Serial.println("\n\n Steps Completed ...");  
    Serial.println("");
    Serial.println("Enter Next Step Count:");  
    }
}

void triggerPulse(int count)
{
  
  for(int i=0;i<count;i++){
  digitalWrite(stepPin,1);
  delay(pulseDelay);
  //delayMicroseconds(pulseDelay);
  digitalWrite(stepPin,0);
  //delayMicroseconds(pulseDelay);
  delay(pulseDelay);
  }}
