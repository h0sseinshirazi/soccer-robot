#include <Adafruit_SH1106_STM32.h>
Adafruit_SH1106 display(-1);

#define ldrSens 400

int i, tsop, num;
int buff[8];
int GY, head, counter = 0;
int shb, shr, shl;
float e;
int v = 45000;
int ldr_F, ldr_B, ldr_R, ldr_L;
int LDR_R, LDR_L, LDR_B, LDR_F;
int out_cnt = 0;
int shootCount = 0;
int shootSens;
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
  // sharp
  shr = analogRead(PA5);
  shb = analogRead(PA6);
  shl = analogRead(PA7);
  e = (shr - shl) * 20;
  // ldr
  ldr_B = LDR_B - analogRead(PA1);
  ldr_R = LDR_R - analogRead(PA2);
  ldr_F = LDR_F - analogRead(PA3);
  ldr_L = LDR_L - analogRead(PA4);


  // set out
  if (digitalRead(PA15) == 1) {
    set_LDR();

    Serial1.write(0xA5);
    Serial1.write(0x54);
    delay(200);
    Serial1.write(0xA5);
    Serial1.write(0x55);
    delay(200);
    Serial1.write(0xA5);
    Serial1.write(0x51);
    while (digitalRead(PA15) == 1)
      ;
  }
  shootSens = analogRead(PB0);
}
void rote() {
  if (head <= 5 && head >= -5) GY = 0;
  else if (head >= 5 && head < 50) GY = head * 250;
  else if (head >= 50 && head < 180) GY = head * 270;
  else if (head <= -5 && head > -50) GY = head * 250;
  else if (head <= -50 && head > -180) GY = head * 270;
}  /////////////set LDR/////////////
void set_LDR() {
  LDR_B = analogRead(PA1);
  LDR_R = analogRead(PA2);
  LDR_F = analogRead(PA3);
  LDR_L = analogRead(PA4);
}
/////////////motor/////////////
void motor(int ML1, int ML2, int MR2, int MR1) {
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
  display.setCursor(0, 0);
  display.print(num);
  display.print(":");
  display.println(tsop);

  display.print("shb :");
  display.println(shb);
  display.print("shl :");
  display.println(shl);

  display.print("shr :");
  display.println(shr);
  display.print("gy :");
  display.println(head);

  display.print("ldr_F :");
  display.println(ldr_F);
  display.print("ldr_B :");
  display.println(ldr_B);
  display.print("ldr_L :");
  display.println(ldr_L);
  display.setCursor(64, 0);
  display.print("ldrR :");
  display.print(ldr_R);
  display.setCursor(64, 10);
  display.print("sh:");
  display.print(shootSens);
  display.setCursor(64, 20);
  display.print("e:");
  display.print(e);
  display.display();
}
////////////move//////////////
void move(int dir) {
  if (dir == 0) motor(v + GY, v + GY, -v + GY, -v + GY);
  else if (dir == 1) motor(v + GY, v / 2 + GY, -v + GY, -v / 2 + GY);
  else if (dir == 2) motor(v + GY, 0 + GY, -v + GY, 0 + GY);
  else if (dir == 3) motor(v + GY, -v / 2 + GY, -v + GY, v / 2 + GY);
  else if (dir == 4) motor(v + GY, -v + GY, -v + GY, v + GY);
  else if (dir == 5) motor(v / 2 + GY, -v + GY, -v / 2 + GY, v + GY);
  else if (dir == 6) motor(0 + GY, -v + GY, 0 + GY, v + GY);
  else if (dir == 7) motor(-v / 2 + GY, -v + GY, v / 2 + GY, v + GY);
  else if (dir == 8) motor(-v + GY, -v + GY, v + GY, v + GY);
  else if (dir == 9) motor(-v + GY, -v / 2 + GY, v + GY, v / 2 + GY);
  else if (dir == 10) motor(-v + GY, 0 + GY, v + GY, 0 + GY);
  else if (dir == 11) motor(-v + GY, v / 2 + GY, v + GY, -v / 2 + GY);
  else if (dir == 12) motor(-v + GY, v + GY, v + GY, -v + GY);
  else if (dir == 13) motor(-v / 2 + GY, v + GY, v / 2 + GY, -v + GY);
  else if (dir == 14) motor(0 + GY, v + GY, 0 + GY, -v + GY);
  else if (dir == 15) motor(v / 2 + GY, v + GY, -v / 2 + GY, -v + GY);
}
////////////moveForSec////////
void moveforSec(int dir, int sec) {
  move(dir);
  for (int i = 0; i < sec; i++) {
    move(dir);
    sensor();
    rote();
    print_all();
  }
}
//////////////out//////////////////
void out() {
  if (shr > 2000) {
    moveforSec(12, 4);
    while (num <= 8 && tsop < 3000) {
      MoveIn();
      sensor();
      rote();
      print_all();
    }
  } else if (shl > 2000) {
    moveforSec(4, 4);
    while (num > 8 && tsop < 3000) {
      MoveIn();
      sensor();
      rote();
      print_all();
    }
  } else if (shb > 1300) {
    moveforSec(0, 4);
    while ((num > 4 || num < 12) && tsop < 3000) {
      MoveIn();
      sensor();
      rote();
      print_all();
    }
  }
}
/////////////LDR_R/////////////////
void out_ldr() {
  out_cnt = 0;
  if (ldr_R > ldrSens) {
    moveforSec(12, 13);
    while (num < 8 && tsop < 4000 && out_cnt < 70) {
      MoveInSide();
      sensor();
      rote();
      print_all();
      out_cnt++;
    }
  }

  /////////////////////////////////////
  else if (ldr_L > ldrSens) {
    moveforSec(4, 13);
    while (num > 8 && tsop < 4000 && out_cnt < 70) {
      MoveInSide();
      sensor();
      rote();
      print_all();
      out_cnt++;
    }
  }
  //////////////////////////////////////
  else if (ldr_B > ldrSens) {
    moveforSec(0, 13);
    while ((num > 4 && num < 12) && tsop < 4000 && out_cnt < 70) {
      MoveInSide();
      sensor();
      rote();
      print_all();
      out_cnt++;
    }
  }
  /////////////////////////////////////
  else if (ldr_F > ldrSens) {
    moveforSec(8, 13);
    while ((num < 4 || num > 12) && tsop < 3000 && out_cnt < 50) {
      MoveInSide(); 
      sensor();
      rote();
      print_all();
      out_cnt++;
    }
  }
  //////////////////////////////////////////////
  else if (ldr_F > ldrSens && ldr_L > ldrSens) {
    moveforSec(6, 13);
    while ((num > 10 || num < 2) && tsop < 3000 && out_cnt < 50) {
      MoveInSide();
      sensor();
      rote();
      print_all();
      out_cnt++;
    }
  }
  ///////////////////////////////////////////////
  else if (ldr_F > ldrSens && ldr_R > ldrSens) {
    moveforSec(10, 5);
    while ((num < 6 || num > 12) && tsop < 3000 && out_cnt < 50) {
      MoveInSide();
      sensor();
      rote();
      print_all();
      out_cnt++;
    }
  }
  ///////////////////////////////////////////////
  else if (ldr_B > ldrSens && ldr_R > ldrSens) {
    moveforSec(14, 5);
    while ((num < 10 && num > 2) && tsop < 3000 && out_cnt < 50) {
      MoveInSide();
      sensor();
      rote();
      print_all();
      out_cnt++;
    }
  }
  //////////////////////////////////////////////
  else if (ldr_B > ldrSens && ldr_L > ldrSens) {
    moveforSec(2, 5);
    while ((num > 6 && num < 12) && tsop < 3000 && out_cnt < 50) {
      MoveInSide();
      sensor();
      rote();
      print_all();
      out_cnt++;
    }
  }
}
///////////////////////////////////

void MoveInSide() {
  if (ldr_R > ldrSens) move(12);
  else if (ldr_L > ldrSens) move(4);
  else if (ldr_B > ldrSens) move(0);
  else if (ldr_F > ldrSens) move(8);
  else if (ldr_F > ldrSens && ldr_R > ldrSens) move(10);
  else if (ldr_F > ldrSens && ldr_L > ldrSens) move(6);
  else if (ldr_B > ldrSens && ldr_R > ldrSens) move(14);
  else if (ldr_B > ldrSens && ldr_L > ldrSens) move(2);
  else motor(GY, GY, GY, GY);
}

/////////////////////////////////////

void MoveIn() {
  if (shr > 2000) move(12);
  else if (shl > 2000) move(4);
  else if (shb > 2000) move(0);
  else motor(GY, GY, GY, GY);
}
//////////////////////////////////////
void shootForSec(int sec){
  for(int i =0; i<sec;i++){
  sensor();
  print_all();
  rote();
  out_ldr();
  }
}
void shoot() {
  if (shootSens > 4000 && shootCount == 0) {
    digitalWrite(PB5, 1);
    shootForSec(1);
    digitalWrite(PB5, 0);
    shootForSec(2);
    shootCount = 1;
  } else if (shootSens < 4000) shootCount = 0;
}
//////////////setup///////////////////
void AI1() {
  out_ldr();
  sensor();
  print_all();
  rote();
  if (tsop < 3000) {
    if (num == 0) move(0);
    else if (num < 8 && num >= 1) move(num + 2);
    else if (num >= 8) move(num - 2);
  }


  else {
    if (shb > 1300) motor(-e + 35000 + GY, e + 35000 + GY, e - 35000 + GY, -e - 35000 + GY);
    else if (shb < 1100) motor(-e - 35000 + GY, e - 35000 + GY, e + 35000 + GY, -e + 35000 + GY);
    else motor(0 + GY, 0 + GY, 0 + GY, 0 + GY);
  }
}
void AI2() {
  sensor();
  if (e > 10000 && tsop < 4000)   head = head - (e/(50000.0))*40; 
  else if (e < -15000 && tsop < 4000)    head = head + (-e/(50000.0))*30;
  else   head = head;

  rote();
  out_ldr();
  shoot();
  print_all();

  
  if (tsop < 4000) {

      if (num == 0) move(0);
      else if (num < 8 && num >= 1) move(num + 2);
      else if (num >= 8) move(num - 2);
    } 
  
  else {
    if (shb > 1200)          motor(-e + 35000 + GY, e + 35000 + GY, e - 35000 + GY, -e - 35000 + GY);
    else if (shb < 1100)     motor(-e - 35000 + GY, e - 35000 + GY, e + 35000 + GY, -e + 35000 + GY);
    else                     motor(0 + GY, 0 + GY, 0 + GY, 0 + GY);
  }
}
void setup() {
  pinMode(PA15, INPUT);
  pinMode(PA12, INPUT);

  pinMode(PB15, OUTPUT);
  pinMode(PB9, PWM);


  pinMode(PB14, OUTPUT);
  pinMode(PB8, PWM);

  pinMode(PB13, OUTPUT);
  pinMode(PB7, PWM);

  pinMode(PB12, OUTPUT);
  pinMode(PB6, PWM);
  motor(0, 0, 0, 0);
  //
  pinMode(PB5, OUTPUT);
  //
  pinMode(PA8, OUTPUT);
  pinMode(PB1, OUTPUT);
  pinMode(PC14, OUTPUT);
  pinMode(PC15, OUTPUT);
  pinMode(PA1, INPUT);
  pinMode(PA2, INPUT);
  pinMode(PA3, INPUT);
  pinMode(PA4, INPUT);

  display.begin(0x2, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(40, 40);
  display.print("loading...");
  display.display();
  delay(100);
  Serial1.begin(115200);
  Serial1.write(0xA5);
  Serial1.write(0x54);
  delay(500);
  Serial1.write(0xA5);
  Serial1.write(0x55);
  delay(500);
  Serial1.write(0xA5);
  Serial1.write(0x51);
  set_LDR();
}
///////////////////////////

void loop() {
  AI2();
  //AI1();
}
////////////////////////////

