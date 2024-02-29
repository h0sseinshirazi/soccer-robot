#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);

int i, tsop, num;
int buff[8];
float GY, head;
int counter = 0;
int v = 40000;
int shb, shr, shl;
int e;


////////////sensor////////////
void sensor() {
  tsop = 4095;
  for (int i = 0; i < 16; i++) {
    digitalWrite(PA8, (i / 1) % 2);
    digitalWrite(PB1, (i / 2) % 2);
    digitalWrite(PC14, (i / 4) % 2);
    digitalWrite(PC15, (i / 8) % 2);
    if (analogRead(PA0) < tsop) {
      tsop = analogRead(PA0);
      num = i;
    }
  }

  Serial1.write(0xA5);
  Serial1.write(0x51);
  while (true) {
    buff[counter] = Serial1.read();
    if (counter == 0 && buff[0] != 0xAA) break;

    counter++;
    if (counter == 8) {
      counter = 0;
      if (buff[0] == 0xAA && buff[7] == 0x55) {
        head = (int16_t)(buff[1] << 8 | buff[2]) / 100;
      }
    }
  }
  GY = head * 250;
  shr = analogRead(PA6);
  shb = analogRead(PA5);
  shl = analogRead(PA7);
}  /////////////motor/////////////
void motor(int ML1, int ML2, int MR2, int MR1) {

  ML1 += GY;
  ML2 += GY;
  MR2 += GY;
  MR1 += GY;
  
  if (ML1 > 65535) ML1 = 65535;
  if (ML2 > 65535) ML2 = 65535;
  if (MR2 > 65535) MR2 = 65535;
  if (MR1 > 65535) MR1 = 65535;
  if (ML1 < -65535) ML1 = -65535;
  if (ML2 < -65535) ML2 = -65535;
  if (MR2 < -65535) MR2 = -65535;
  if (MR1 < -65535) MR1 = -65535;  
  ///ML1

  if (ML1 > 0) {
    digitalWrite(PB15, 0);
    pwmWrite(PB9, ML1);
  } else {
    digitalWrite(PB15, 1);
    pwmWrite(PB9, ML1 + 65535);
  }
  ///ML2

  if (ML2 > 0) {
    digitalWrite(PB14, 0);
    pwmWrite(PB8, ML2);
  } else {
    digitalWrite(PB14, 1);
    pwmWrite(PB8, ML2 + 65535);
  }
  ///MR2

  if (MR2 > 0) {
    digitalWrite(PB13, 0);
    pwmWrite(PB7, MR2);
  } else {
    digitalWrite(PB13, 1);
    pwmWrite(PB7, MR2 + 65535);
  }
  ///MR1

  if (MR1 > 0) {
    digitalWrite(PB12, 0);
    pwmWrite(PB6, MR1);
  } else {
    digitalWrite(PB12, 1);
    pwmWrite(PB6, MR1 + 65535);
  }
}
///////////////print_all///////////////
void print_all() {
  display.clearDisplay();
  display.setCursor(0, 27);
  display.println(head);

  display.setCursor(30, 25);

  display.print("shb :");
  display.println(shb);
  display.print("shl :");
  display.println(shl);
  display.print("shr :");
  display.println(shr);

  display.setTextSize(1);
  display.setCursor(64, 10);
  display.display();
}
////////////moveForSec////////
////////////move//////////////
void move(int dir) {
  if (dir == 0) motor(v, v, -v, -v);
  else if (dir == 1) motor(v, v / 2, -v, -v);
  else if (dir == 2) motor(v, 0, -v, 0);
  else if (dir == 3) motor(v, -v / 2, -v, v / 2);
  else if (dir == 4) motor(v, -v, -v, v);
  else if (dir == 5) motor(v / 2, -v, -v / 2, v);
  else if (dir == 6) motor(0, -v, 0, v);
  else if (dir == 7) motor(-v, 0, v / 2, v);
  else if (dir == 8) motor(-v, -v, v, v);
  else if (dir == 9) motor(-v, -v / 2, v, v / 2);
  else if (dir == 10) motor(-v, 0, v, 0);
  else if (dir == 11) motor(-v, v / 2, v, -v / 2);
  else if (dir == 12) motor(-v, v, v, -v);
  else if (dir == 13) motor(-v / 2, v, v / 2, -v);
  else if (dir == 14) motor(0, v, 0, -v);
  else if (dir == 15) motor(v / 2, v, -v / 2, -v);
}
//////////////////////////////

//////////////setup///////////////////
void setup() {
  pinMode(PB15, OUTPUT);
  pinMode(PB9, PWM);


  pinMode(PB14, OUTPUT);
  pinMode(PB8, PWM);

  pinMode(PB13, OUTPUT);
  pinMode(PB7, PWM);

  pinMode(PB12, OUTPUT);
  pinMode(PB6, PWM);

  pinMode(PA8, OUTPUT);
  pinMode(PB1, OUTPUT);
  pinMode(PC14, OUTPUT);
  pinMode(PC15, OUTPUT);
  pinMode(PA5,INPUT);
  pinMode(PA6,INPUT);
  pinMode(PA5,INPUT);
  

  display.begin(0x2, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(40, 40);
  display.print("loading...");
  display.display();
  delay(1000);
  Serial1.begin(115200);
  Serial1.write(0xA5);
  Serial1.write(0x54);
  delay(1000);
  Serial1.write(0xA5);
  Serial1.write(0x51);
e=shr-shl;
e*=12;
}


void loop() {
  sensor();
  print_all();
    if (head <= 5 && head >=-5) GY = 0;
    else if (head >= 5 && head < 50) GY = head * 250;
    else if (head >= 50 && head < 180) GY = head * 270;
   else if (head <= -5 && head > -50) GY = head * 250;
    else if (head <= -50 && head > -180) GY = head * 270;

  if (tsop < 4000) {
    if (num == 0) move(0);
    else if (num < 8) move(num + 2);
    else if (num >= 8) move(num - 2);

  } 
    if(shb<700) motor(-v-e,-v+e,v+e,v-e);
        else{motor(GY,GY,GY,GY);}
  
}