String inputData2;
int input_data2;


//uart1 STM32<---->ESP32
//uart2 STM32<---->TTL
void stm32_uart1() {
  while (Serial1.available() > 0) {
    inputData = Serial1.readStringUntil('\n');
    inputData.trim();
    Serial2.println(inputData);
    check_();
    inputData = "";
    input_data = 0;
  }
}

void stm32_uart2() {
  while (Serial2.available() > 0) {
    inputData2 = Serial2.readStringUntil('\n');
    inputData2.trim();
    Serial2.println(inputData2);
    if (serialMode == 1) Serial1.println(inputData2);
    inputData2 = "";
    input_data2 = 0;
  }
}