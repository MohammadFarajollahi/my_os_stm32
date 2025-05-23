String inputData2;
int input_data2;


//uart1 ---->ESP32
//uart2 ---->TTL
//uart3 ---->GSM
void stm32_uart1() {  //esp32  Uart
  while (Serial1.available() > 0) {
    inputData = Serial1.readStringUntil('\n');
    inputData.trim();
    Serial2.println(inputData);
    check_();
    jasonCheck();
    inputData = "";
    input_data = 0;
  }
}

void stm32_uart2() {
  while (Serial2.available() > 0) {
    inputData2 = Serial2.readStringUntil('\n');
    inputData2.trim();

    
    //*****clock Setting*****
    String cmd = inputData2;
    if (cmd.startsWith("SET")) {
      int y, m, d, h, min, s;
      if (sscanf(cmd.c_str(), "SET %d %d %d %d %d %d", &y, &m, &d, &h, &min, &s) == 6) {
        rtc.adjust(DateTime(y, m, d, h, min, s));
        Serial.println("زمان با موفقیت تنظیم شد.");
      } else {
        Serial.println("فرمت نادرست. مثال: SET 2025 05 18 14 25 00");
      }
    }

    if (p2p == 1) {
      Serial3.println(inputData2);
    }
    if (inputData2 == "p2p on" || inputData2 == "P2P ON") {
      Serial2.println("p2p turned on");
      p2p = 1;
    }

    Serial2.println(inputData2);
    if (serialMode == 1) Serial1.println(inputData2);
    inputData2 = "";
    input_data2 = 0;
  }
}