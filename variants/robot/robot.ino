void setup() {
  // put your setup code here, to run once:
  pinMode(PB15,OUTPUT);
  pinMode(PB14,OUTPUT);
  pinMode(PB13,OUTPUT);
  pinMode(PB12,OUTPUT); 




  pinMode(PA8,PWM);
  pinMode(PB8,PWM);
  pinMode(PB7,PWM);
  pinMode(PB6,PWM);


}

void loop() {
  // put your main code here, to run repeatedly:
  /////////mr1
   digitalWrite(PB12,0);
      pwmWrite(PB6,10000);  

  /////mr2
  digitalWrite(PB13,0);
      pwmWrite(PB7,10000);

 /////mL2
   digitalWrite(PB14,0);
      pwmWrite(PB8,10000);

 /////mL1
      digitalWrite(PB15,0);
      pwmWrite(PA8,10000);

  delay(1000)

  gc
    // put your main code here, to run repeatedly:
  /////////mr1
   digitalWrite(PB12,0);
      pwmWrite(PB6,10000);  

  /////mr2
  digitalWrite(PB13,0);
      pwmWrite(PB7,10000);

 /////mL2
   digitalWrite(PB14,0);
      pwmWrite(PB8,10000);

 /////mL1
      digitalWrite(PB15,0);
      pwmWrite(PA8,10000);
}



  delay(1000);

/////////////////////////////////////////////////

///////MR1
  digitalWrite(PB12,1);
  pwmWrite(PB6,55000); 
  ///////MR2
  digitalWrite(PB13,1);
  pwmWrite(PB7,55000);
 ////ML2
  digitalWrite(PB14,1);
  pwmWrite(PB8,55000);
 ////ML1
  digitalWrite(PB15,1);
  pwmWrite(PA8,55000) ;

delay(1000)

