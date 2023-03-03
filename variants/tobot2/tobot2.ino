#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);
int buff[8];
int counter=0;
float GY;
int Min,number;
int shb,shr,shl;
int v=200;
int d;

void motor(int ML1,int ML2,int MR2,int MR1){
  MR1=MR1*255;
  MR2=MR2*255; 
  ML1=ML1*255;
  ML2=ML2*255;
 3
 
//////
  if(ML1>= 0){
    digitalWrite(PB15,0);
    pwmWrite(PA8,ML1);
  }
  else{
    digitalWrite(PB15,1);
    pwmWrite(PA8,ML1+65535);    
  }
//////
  if(ML2>= 0){
    digitalWrite(PB14,0);
    pwmWrite(PB8,ML2);
  }
  else{
    digitalWrite(PB14,1);
    pwmWrite(PB8,ML2+65535);    
  }
//////
  if(MR2>= 0){
    digitalWrite(PB13,0);
    pwmWrite(PB7,MR2);
  }
  else{
    digitalWrite(PB13,1);
    pwmWrite(PB7,MR2+65535);    
  }
//////
  if(MR1>= 0){
    digitalWrite(PB12,0);
    pwmWrite(PB6,MR1);
  }
  else{
    digitalWrite(PB12,1);
    pwmWrite(PB6,MR1+65535);    
  }  
  
}

void sensor(){
   Min=4095;
   for(int i=0;i<16;i++){
    digitalWrite(PB1,(i/1)%2);
    digitalWrite(PB0,(i/2)%2);
    digitalWrite(PC14,(i/4)%2);
    digitalWrite(PC15,(i/8)%2);
    if(analogRead(PA0)<Min)
    {
      Min=analogRead(PA0);
      number=i;
    }
    shb=analogRead(PA1);
    shr=analogRead(PA2);
    shl=analogRead(PA3);
    d=shl-shr;
    
  }
////////////////////////////////////OLED  &&  GY
/////////////////////////////
  Serial1.write(0xA5);
  Serial1.write(0x51);
  while(true)
  {
  buff[counter]=Serial1.read();
  if(counter==0 && buff[0]!=0xAA)  break;
  counter++;
  if(counter==8)
  {
   counter=0;
    if(buff[0]==0xAA && buff[7]==0x55)
    {
    GY=(int16_t)(buff[1]<<8|buff[2])/100;
    }}} 
}


//////////////////////////////////////////////////print_all
void print_all()
{
   display.clearDisplay();   
  //////////////////////////print-gy
   display.setCursor(0,0);
   display.print(GY);   
  ///////////////////////////////print-tsop   
  display.setCursor(0,10);
  display.print(number);
  display.print(": ");
  display.print(Min);
  ///////////////////////////////sharp   
  display.setCursor(0,20);
  display.print("B: ");
  display.print(shb);
  display.setCursor(0,30);
  display.print("R: ");
  display.print(shr);
  display.setCursor(0,40);
  display.print("L: ");
  display.print(shl);
  //////////////////////////////Circle
  display.drawCircle(95, 32, 18, WHITE);
  int xb = 95 + 25 * sin(number * 22.5 * PI/180);
  int yb = 32 - 25 * cos(number * 22.5 * PI/180);
  if(Min < 4000)
    display.fillCircle(xb, yb, 5, WHITE);

  //////////////////////////// GY
  int x1 = 95 + 13 * sin(GY * PI/180);
  int y1 = 32 - 13 * cos(GY * PI/180);
  int x2 = 95 - 13 * sin(GY * PI/180);
  int y2 = 32 + 13 * cos(GY * PI/180);
  display.drawLine(x1, y1, x2, y2, WHITE);
  display.fillCircle(x2, y2, 2, WHITE);
  
  ///////////////////////
  display.display();
}
void setup() 
{
//////motor
 pinMode(PB12,OUTPUT);
 pinMode(PB13,OUTPUT);
 pinMode(PB14,OUTPUT);
 pinMode(PB15,OUTPUT);

/////pwm
 pinMode(PA8,PWM);
 pinMode(PB8,PWM);
 pinMode(PB7,PWM);
 pinMode(PB6,PWM);

////adress mux
  pinMode(PB1,OUTPUT);
  pinMode(PB0,OUTPUT);
  pinMode(PC14,OUTPUT);
  pinMode(PC15,OUTPUT);
 ////////////////////////////////OLED
  display.begin(0x2, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(40,40);
  display.print("loading..");
  display.display();
  //////////////////////GY
  delay(1000); 
  Serial1.begin(115200);
  Serial1.write(0xA5);
  Serial1.write(0x54);
  delay(1000);
  Serial1.write(0xA5);
  Serial1.write(0x51); 
  
  motor(0,0,0,0);
}
`
//////////////////////////////////////////////////////////////
void loop()
{
  sensor();
  print_all(); 
  if(Min<4000)
  {
    if(number==0)          motor(v+GY,v+GY,-v+GY,-v+GY);
    else if(number == 1)   motor(v+GY,v/2+GY,-v+GY,-v/2+GY);
    else if(number == 2)   motor(v+GY,0+GY,-v+GY,0+GY);
    else if(number == 3)   motor(v+GY,-v/2+GY,-v+GY,v/2+GY);
    else if(number == 4)   motor(v+GY,-v+GY,-v+GY,v+GY);
    else if(number == 5)   motor(v/2+GY,-v+GY,-v/2+GY,v+GY);
    else if(number == 6)   motor(0+GY,-v+GY,0+GY,v+GY);
    else if(number == 7)   motor(-v/2+GY,-v+GY,v/2+GY,v+GY);
    else if(number == 8)   motor(-v+GY,-v+GY,v+GY,v+GY);
    else if(number == 9)   motor(-v+GY,-v/2+GY,v+GY,v/2+GY);
    else if(number == 10)  motor(-v+GY,0+GY,v+GY,0+GY);
    else if(number == 11)  motor(-v+GY,v/2+GY,v+GY,-v/2+GY);
    else if(number == 12)  motor(-v+GY,v+GY,v+GY,-v+GY);
    else if(number == 13)  motor(-v/2+GY,v+GY,v/2+GY,-v+GY);
    else if(number == 14)  motor(0+GY,v+GY,0+GY,-v+GY);
    else if(number == 15)  motor(v/2+GY,v+GY,-v/2+GY,-v+GY);
  }
  else
  {
   if(shb<1500)                motor(-v+GY+d, -v+GY-d, v+GY-d, v+GY+d);
   else                      motor(0+GY,0+GY,0+GY,0+GY);
}
}
