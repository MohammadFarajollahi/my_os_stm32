void timer_(void) {
  
  digitalWrite(led, !digitalRead(led));

  if (GsmReady == 0) {
    ++GsmReadyTimer;
    digitalWrite(gsmLed, !digitalRead(gsmLed));
  }

  if (GsmReady == 1){
    digitalWrite(gsmLed, 1);
    if(messageSendCheck == 1)++messageSendTimer;
    if(smsCheck == 1)++SMScheckTimer;
  }

  ++esp32chek;
  if(esp32chek >= 150)iwdg_init(IWDG_PRE_256, 1);
}