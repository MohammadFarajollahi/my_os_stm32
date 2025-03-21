
void gsm() {
  while (Serial3.available() > 0) {
    GsmData = Serial3.readStringUntil('\n');
    GsmData.trim();
    Serial2.println(GsmData);
    if (GsmReady == 0) GsmReady = 1;
    Gsm_check();
    GsmData = "";
  }

  if (smsReceived == 1 && GsmReady == 1) {
    smsReceived = 0;  // ریست کردن فلگ
    readSMS();        // تابع خواندن پیامک
    Serial2.println("sms");
  }


  if (GsmReady == 0) firstStart();
  if (GsmReady == 1) {
    if (messageSendCheck == 1 && messageSendTimer >= 10) {
      Serial1.println("sms not send");
      messageSendTimer = 0;
      messageSendCheck = 0;
    }
  }
}

void Gsm_check() {
  sub1 = GsmData.substring(0, 5);  ///+CMGS: 112
  if (sub1 == "+CMGS") {
    messageSendCheck = 0;
    messageSendTimer = 0;
    Serial1.println("sms send");
    Serial2.println("**sms send**");
  }
}

//***************************//
void readSMS() {
  Serial3.println("AT+CMGR=0");
}

//******sendSMS*****
void sendSMS(String PhoneNumber, String TextMessage) {
  for (int i = 0; i <= 10; i++) {
    digitalWrite(gsmLed, !digitalRead(gsmLed));
    delay(50);
  }
  String n = "AT+CMGS=\"" + PhoneNumber + "\"";
  Serial3.println(n);
  Serial2.println(n);
  delay(300);
  Serial3.print(TextMessage);
  delay(200);
  Serial3.write(26);
  delay(200);
  Serial3.write(26);
  delay(200);
  Serial3.write(26);
  delay(200);
  Serial2.println("send sms");
  Serial2.println("send sms");
  messageSendCheck = 1;
}

//*****************firstStart**************
void firstStart() {
  if (GsmReadyCount == 0 && GsmReadyTimer >= 6) {
    Serial2.println("Check GSM");
    GsmReadyCount = 1;
    GsmReadyTimer = 0;
    Serial3.println("AT");
    delay(100);
    Serial3.println("AT");
    delay(100);
    Serial3.println("AT");
  }
  if (GsmReadyCount == 1 && GsmReadyTimer >= 2) {
    Serial2.println("GSM off");
    digitalWrite(GSMPower, 1);
    delay(2000);
    digitalWrite(GSMPower, 0);
    delay(2000);
    GsmReadyCount = 0;
    GsmReadyTimer = 0;
  }

  if (GsmReadyCount == 2) {
    Serial2.println("GSM ON");
    Serial2.println("Config GSM Madule");
    Serial3.println("AT+CMGF=1");
    delay(100);
    Serial3.println("AT+CSMP=17,167,0,0");
    delay(100);
    Serial3.println("AT+CSCS=\"GSM\"");
    delay(100);
    Serial3.println("ATE0");
    delay(100);
    Serial3.println("AT&W");
    delay(100);
    GsmReady = 1;
    GsmReadyCount = 3;
    smsReceived = 0;
    GsmReadyTimer = 0;
  }
}