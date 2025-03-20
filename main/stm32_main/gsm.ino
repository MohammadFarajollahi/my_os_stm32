String inGsm;
void gsm() {
  while (Serial3.available() > 0) {
    GsmData = Serial3.readStringUntil('\n');
    GsmData.trim();
    Serial2.println(GsmData);
    if (GsmReady == 0) GsmReady = 1;
    Gsm_check();
    GsmData = "";
  }
  // while (Serial3.available()) {
  //   char c = Serial3.read();  // خوندن تک‌تک کاراکترها
  //   inGsm += c;
  //   delayMicroseconds(200);  // تاخیر کوتاه برای جلوگیری از از دست رفتن دیتا
  // }

  // if (inGsm.length() > 0) {  // وقتی که دیتا دریافت شد
  //   inGsm.trim();
  //   GsmData = inGsm;
  //   Serial2.println(GsmData);  // نمایش داده‌ها در پورت سریال
  //   Gsm_check();               // پردازش پیامک
  //   GsmData = "";              // پاک کردن رشته برای دریافت پیام بعدی
  // }


  if (smsReceived == 1 && GsmReady == 1) {
    smsReceived = 0;  // ریست کردن فلگ
    readSMS();        // تابع خواندن پیامک
    Serial2.println("sms");
  }


  if (GsmReady == 0) {
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
}

void Gsm_check() {
  // if (GsmReady == 0) {
  //   if (GsmReadyCount == 1) {
  //     if (GsmData == "OK" || GsmData == "+CFUN: 1" || GsmData == "+QUSIM: 1" || GsmData == "+CPIN: READY" || GsmData == "+QIND: SMS DONE" || GsmData == "+QIND: PB DONE") {
  //       GsmReadyTimer = 0;
  //       GsmReadyCount = 2;
  //     }
  //   }
  // }
}

//***************************//
void readSMS() {
  Serial3.println("AT+CMGL=\"ALL\"");
}