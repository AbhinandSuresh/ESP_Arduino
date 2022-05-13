void setup() 
{
   Serial.begin(9600);
}

void loop() 
{
  String buff,buff2;
  int a,b,sum,diff,index,pro;
  float quo;
  while(!Serial.available());
  buff=Serial.readString();
  buff.toLowerCase();
  
  if(buff.substring(0,5)=="hello")
  {
    Serial.println("Hai from Arduino");  
  }
  else if(buff.substring(0,3)=="add")
  {
    index=buff.indexOf(" ");
    buff2=buff.substring(index+1); // substring with numbers and space in between
    a=buff2.toInt();
    index=buff2.indexOf(" ");
    b=buff2.substring(index+1).toInt();//second number
    sum=a+b;
    Serial.print(a);
    Serial.print(" + ");
    Serial.print(b);
    Serial.print(" = ");
    Serial.println(sum);
  }
  else if(buff.substring(0,3)=="sub")
  {
    index=buff.indexOf(" ");
    buff2=buff.substring(index+1); // substring with numbers and space in between
    a=buff2.toInt();
    index=buff2.indexOf(" ");
    b=buff2.substring(index+1).toInt();//second number
    diff=a-b;
    Serial.print(a);
    Serial.print(" - ");
    Serial.print(b);
    Serial.print(" = ");
    Serial.println(diff);
  }
  else if(buff.substring(0,3)=="mul")
  {
    index=buff.indexOf(" ");
    buff2=buff.substring(index+1); // substring with numbers and space in between
    a=buff2.toInt();
    index=buff2.indexOf(" ");
    b=buff2.substring(index).toInt();//second number
    pro=a*b;
    Serial.print(a);
    Serial.print(" x ");
    Serial.print(b);
    Serial.print(" = ");
    Serial.println(pro);
  }
   else if(buff.substring(0,3)=="div")
  {
    index=buff.indexOf(" ");
    buff2=buff.substring(index+1); // substring with numbers and space in between
    a=buff2.toInt();
    index=buff2.indexOf(" ");
    b=buff2.substring(index+1).toInt();//second number
    float c =(float)(a);
    float d =(float)(b); 
    quo=c/d;
    Serial.print(a);
    Serial.print(" / ");
    Serial.print(b);
    Serial.print(" = ");
    Serial.println(quo);
  }
  else
  {
    Serial.print(buff);
    Serial.println("Received.");
    Serial.println("Error; Menu: ADD/SUB/MUL/DIV <space> number <space> number");
  }
}
