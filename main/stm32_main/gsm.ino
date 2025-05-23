
void gsm() {
  while (Serial3.available() > 0) {
    GsmData = Serial3.readStringUntil('\n');
    GsmData.trim();
    Serial2.println(GsmData);
    Gsm_check();
    GsmData = "";
  }


  if (smsReceived == 1 && GsmReady == 1) {
    smsReceived = 0;  // ریست کردن فلگ
    readSMS();        // تابع خواندن پیامک
    Serial2.println("sms");
  }


  if (GsmReady == 0) firstStart();
  //***********************GSM Ready***********************
  if (GsmReady == 1) {
    if (messageSendCheck == 1 && messageSendTimer >= 30) {
      Serial1.println("sms not send");
      messageSendTimer = 0;
      messageSendCheck = 0;
    }


    ////////sms Check/////////
    if (smsCheck == 1 && SMScheckTimer >= 2) {
      SMScheckTimer = 0;
      ++smsCheckCount;
      Serial3.println("AT+CMGL=\"ALL\"");
      Serial2.println("Read SMS again...");

      if (smsCheckCount >= 3) {
        smsCheckCount = 0;
        smsCheck = 0;
        SMScheckTimer = 0;
        Serial2.println("No SMS");
        Serial2.println("Dellet All Message");
        Serial3.println("AT+CMGD=1,4");
        delay(1000);
        Serial3.println("AT+CSMP=17,167,0,0");
        delay(500);
        Serial3.println("AT+CSCS=\"GSM\"");
        delay(500);
      }
    }
  }
}



//*****************************************************************GSM CHECK*******************************************************************
//*****************************************************************GSM CHECK*******************************************************************
//*****************************************************************GSM CHECK*******************************************************************
void Gsm_check() {

  if (GsmReady == 0) {
    if (GsmData == "OK" && GsmReadyCount == 1) GsmReadyCount = 2;
    if (GsmData == "+CREG: 0,1" && GsmReadyCount == 3) GsmReadyCount = 4;
    sub1 = GsmData.substring(12, 18);  ///+COPS: 0,0,"IR-TCI",7
    if (GsmReadyCount == 5) {
      if (sub1 == "IR-TCI" || sub1 == "IR-MCI") {  //
        simModel = 2;
        Serial2.println("Sim Model:MCI");
        GsmReadyCount = 6;
      }
    }
  }

  //******READ SMS*****
  sub1 = GsmData.substring(0, 5);  ///+CMTI: "ME",0
  if (sub1 == "+CMTI") {
    readSMS();
  }

  sub1 = GsmData.substring(0, 5);  ///+CMGL: 0,"REC UNREAD","+989114764806",,"25/05/07,01:52:03+14"
  if (sub1 == "+CMGL") {
    sub1 = GsmData.substring(10, 20);  ///+CMGL: 0,"REC UNREAD","+989114764806",,"25/05/07,01:52:03+14"
    if (sub1 == "REC UNREAD") {
      InputNumber = GsmData.substring(23, 36);
      MessNum = GsmData.substring(7, 8);
      inputMessage = Serial3.readStringUntil('\n');
      inputMessage.trim();
      Serial2.print("Message Number:");
      Serial2.println(MessNum);
      Serial2.print("Input Number:");
      Serial2.println(InputNumber);
      Serial2.print("Messsage:");
      Serial2.println(inputMessage);
      ///////////////////////////////////
      StaticJsonDocument<200> jsonDoc;
      jsonDoc["type"] = "ReciveSMS";      // نوع دیتا (مثلاً ارسال پیامک)
      jsonDoc["phone"] = InputNumber;     // شماره تلفن
      jsonDoc["message"] = inputMessage;  // متن پیام
      jsonDoc["extra"] = "urgent";        // هر دیتای اضافی
      String jsonString;
      serializeJson(jsonDoc, jsonString);
      Serial2.println(jsonString);
      Serial1.println(jsonString);
      //////////////////////////////////////
      smsCheckCount = 0;
      smsCheck = 0;
      SMScheckTimer = 0;
      // Serial3.println("AT+CMGD=1,4");
      String s = "AT+CMGD=" + MessNum;
      Serial3.println(s);
      delay(1000);
      Serial3.println("AT+CSMP=17,167,0,0");
      delay(500);
      Serial3.println("AT+CSCS=\"GSM\"");
      delay(500);
      SMScheckTimer = 0;
      smsCheck = 1;
    }
    sub1 = GsmData.substring(10, 18);  ///+CMGL: 0,"REC READ","+989114764806",,"25/05/07,01:52:03+14"
    if (sub1 == "REC READ") {
      InputNumber = GsmData.substring(21, 34);
      MessNum = GsmData.substring(7, 8);
      inputMessage = Serial3.readStringUntil('\n');
      inputMessage.trim();
      Serial2.print("Message Number:");
      Serial2.println(MessNum);
      Serial2.print("Input Number:");
      Serial2.println(InputNumber);
      Serial2.print("Messsage:");
      Serial2.println(inputMessage);
      ///////////////////////////////////
      StaticJsonDocument<200> jsonDoc;
      jsonDoc["type"] = "ReciveSMS";      // نوع دیتا (مثلاً ارسال پیامک)
      jsonDoc["phone"] = InputNumber;     // شماره تلفن
      jsonDoc["message"] = inputMessage;  // متن پیام
      jsonDoc["extra"] = "urgent";        // هر دیتای اضافی
      String jsonString;
      serializeJson(jsonDoc, jsonString);
      Serial2.println(jsonString);
      Serial1.println(jsonString);
      //////////////////////////////////////
      smsCheckCount = 0;
      smsCheck = 0;
      SMScheckTimer = 0;
      // Serial3.println("AT+CMGD=1,4");
      String s = "AT+CMGD=" + MessNum;
      Serial3.println(s);
      delay(1000);
      Serial3.println("AT+CSMP=17,167,0,0");
      delay(500);
      Serial3.println("AT+CSCS=\"GSM\"");
      delay(500);
      SMScheckTimer = 0;
      smsCheck = 1;
    }
  }

  //******SMS SEND********
  sub1 = GsmData.substring(0, 5);  ///+CMGS: 112
  if (sub1 == "+CMGS") {
    messageSendCheck = 0;
    messageSendTimer = 0;
    Serial1.println("sms send");
    Serial2.println("**sms send**");
  }
  //*******SMS ERROR********
  if (GsmData == "+CMS ERROR: 304") {
    Serial2.println("sms storage full");
    Serial3.println("AT+CMGD=1,4");
    delay(500);
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
  }

  if (GsmData == "+CMS ERROR: 332") {
    Serial2.println("SMS NOT SEND");
    //Serial1.println("SMS ERROR");
  }

  sub1 = GsmData.substring(0, 10);  ///+CMS ERROR:
  if (sub1 == "+CMS ERROR") {
    Serial1.println("SMS ERROR");
  }
}


//***************************//
void readSMS() {
  Serial3.println("AT+CMGL=\"ALL\"");
  smsCheck = 1;
}

////********************************************************************sendSMS**************************************************************
////********************************************************************sendSMS**************************************************************
////********************************************************************sendSMS**************************************************************
void sendSMS(String PhoneNumber, String TextMessage) {
  for (int i = 0; i <= 10; i++) {
    digitalWrite(gsmLed, !digitalRead(gsmLed));
    delay(50);
  }
  String n = "AT+CMGS=\"" + PhoneNumber + "\"";
  Serial3.println(n);
  Serial2.println(n);
  delay(100);
  Serial3.flush();
  while (Serial3.available()) Serial3.read();
  delay(100);
  Serial3.print(TextMessage);
  delay(500);
  Serial3.write(26);
  delay(100);
  Serial3.write(26);
  delay(100);
  Serial3.write(26);
  delay(100);
  Serial3.write(26);
  delay(100);
  Serial3.write(26);
  delay(100);
  messageSendCheck = 1;
  Serial2.println("wait for Send...");
  Serial1.println("wait for Send...");
}




//********************************************************firstStart********************************************************
//********************************************************firstStart********************************************************
//********************************************************firstStart********************************************************
void firstStart() {
  if (GsmReadyCount == 0 && GsmReadyTimer >= 10) {
    Serial2.println("Check GSM");
    GsmReadyCount = 1;
    GsmReadyTimer = 0;
    Serial3.println("AT");
    delay(50);
    Serial3.println("AT");
    delay(50);
    Serial3.println("AT");
  }
  if (GsmReadyCount == 1 && GsmReadyTimer >= 5) {
    Serial2.println("GSM off");
    digitalWrite(GSMPower, 1);
    delay(2000);
    digitalWrite(GSMPower, 0);
    delay(3000);
    GsmReadyCount = 0;
    GsmReadyTimer = 0;
  }

  if (GsmReadyCount == 2) {
    Serial3.println("AT+CREG?");
    GsmReadyCount = 3;
    GsmReadyTimer = 0;
  }

  if (GsmReadyCount == 3 && GsmReadyTimer >= 6) {
    Serial3.println("AT+CREG?");
    GsmReadyCount = 3;
    GsmReadyTimer = 0;
    Serial2.println("GSM Not Connect");
    ++gsmError;
  }

  if (gsmError >= 4) {
    gsmError = 0;
    Serial2.println("Reset Gsm modem");
    Serial2.println("GSM off");
    digitalWrite(GSMPower, 1);
    delay(2000);
    digitalWrite(GSMPower, 0);
    delay(2000);
    GsmReadyCount = 0;
    GsmReadyTimer = 0;
  }

  if (GsmReadyCount == 4) {
    gsmError = 0;
    Serial2.println("Connected To network");
    GsmReadyCount = 5;
    GsmReadyTimer = 0;
    Serial3.println("AT+COPS?");
  }


  if (GsmReadyCount == 5 && GsmReadyTimer >= 2) {
    Serial2.println("Sim Model Error");
    GsmReadyCount = 5;
    GsmReadyTimer = 0;
    Serial3.println("AT+COPS?");
    gsmError++;
  }

  if (GsmReadyCount == 6) {
    Serial2.println("SimCard OK");
    GsmReadyCount = 7;
    GsmReadyTimer = 0;
    gsmError = 0;
  }


  if (GsmReadyCount == 7) {
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
    Serial3.println("AT+CMGD=1,4");
    GsmReady = 1;
    GsmReadyCount = 8;
    smsReceived = 0;
    GsmReadyTimer = 0;
    Serial2.println("GSM READY && START");
    Serial1.println("network_ready");
  }
}
