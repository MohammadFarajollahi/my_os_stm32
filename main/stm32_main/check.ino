

void check_() {

  //****check for ready****
  if (inputData == "stm32-check") {
    esp32chek = 0;
    Serial1.println("stm32_ready");
    Serial2.println("esp32 check for ready");
  }

  if (inputData == "checkStm32") {
    esp32chek = 0;
    Serial1.println("stm32 ok");
    Serial2.println("esp32 ok");
  }

  if (inputData == "GET") {
    DateTime now = rtc.now();
    int jy, jm, jd;
    gregorianToJalali(now.year(), now.month(), now.day(), &jy, &jm, &jd);
    int wd = now.dayOfTheWeek();  // 0 = Sunday
    // ارسال داده به صورت CSV
    Serial1.print("clockRead:");
    Serial1.print(jy);
    Serial1.print(",");
    Serial1.print(jm);
    Serial1.print(",");
    Serial1.print(jd);
    Serial1.print(",");
    Serial1.print(now.hour());
    Serial1.print(",");
    Serial1.print(now.minute());
    Serial1.print(",");
    Serial1.print(now.second());
    Serial1.print(",");
    Serial1.println(wd);
  }

  if (inputData.startsWith("clockSet:")) {
    String cmd;
    cmd = inputData.substring(strlen("clockSet:"));

    int values[7];
    int idx = 0;
    char *token = strtok((char *)cmd.c_str(), ",");
    while (token != NULL && idx < 7) {
      values[idx++] = atoi(token);
      token = strtok(NULL, ",");
    }

    if (idx == 7) {
      int jy = values[0], jm = values[1], jd = values[2];
      int hour = values[3], min = values[4], sec = values[5];
      int wd = values[6];

      // تبدیل به میلادی
      int gy, gm, gd;
      jalaliToGregorian(jy, jm, jd, &gy, &gm, &gd);

      rtc.adjust(DateTime(gy, gm, gd, hour, min, sec));
      Serial2.println("✅ زمان جدید تنظیم شد.");
    }
  }

  //****check for ready****
  if (inputData == "network-check") {
    if (GsmReady == 1) {
      Serial1.println("network_ready");
      Serial2.println("ESP32 Cheking Network ready");
    }
    if (GsmReady == 0) {
      Serial1.println("network not ready");
      Serial2.println("ESP32 Cheking Network not ready");
    }
  }

  //****go to serial mode****
  if (inputData == "Serial_mode") {
    serialMode = 1;
    Serial1.println("stm32_serial_mode");
    Serial2.println("goto Serial mode");
  }

  //****exit serial mode****
  if (inputData == "exit serial") {
    serialMode = 0;
    Serial1.println("exit serial mode");
    Serial2.println("exit Serial mode");
  }

  //****atcommand mode****
  if (inputData == "atcommand-mode on") {
    Serial1.println("stm32-atcommandmode on");
    Serial2.println("stm32-atcommandmode on");
    AtCommandMode = 1;
  }

  if (inputData == "atcommand-mode off") {
    Serial1.println("stm32-atcommandmode off");
    Serial2.println("stm32-atcommandmode off");
    AtCommandMode = 0;
  }

  if (AtCommandMode == 1) {  //atcommand  for sed to 4G madule
    sub1 = inputData.substring(0, 9);
    if (sub1 == "atcommand") {
      sub1 = inputData.substring(9, 50);
      Serial2.println(sub1);
    }
  }

  //change buad_rate
  sub1 = inputData.substring(0, 8);  ///baudrate
  if (sub1 == "baudrate") {
    Serial2.flush();
    delay(100);
    Serial2.end();
    delay(100);
    if (inputData == "baudrate:9600") {
      Serial1.println("buadrate 9600");
      Serial2.begin(9600);
    }
    if (inputData == "baudrate:115200") {
      Serial1.println("buadrate 115200");
      Serial2.begin(115200);
    }
    if (inputData == "baudrate:19200") {
      Serial1.println("buadrate 19200");
      Serial2.begin(19200);
    }
    if (inputData == "baudrate:38400") {
      Serial1.println("buadrate 38400");
      Serial2.begin(38400);
    }
    if (inputData == "baudrate:57600") {
      Serial1.println("buadrate 57600");
      Serial2.begin(57600);
    }


    /////
  }
}