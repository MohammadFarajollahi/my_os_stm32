#include <EEPROM.h>
#include <libmaple/iwdg.h>
//****eeprom****
const int addressEEPROM_min = 0;  // Specify the address restrictions you want to use.
const int addressEEPROM_max = 300;
//*****timer****
void timer_(void);
#define LED_RATE 1000000

String inputData;
int input_data;
int serialMode;
String sub1;
int AtCommandMode;

//port_configs
int led = PC13;
void setup() {
  Serial2.begin(9600);
  Serial1.begin(115200);
  Serial2.println("STM32 start...");
  pinMode(led, OUTPUT);

  //******timer*******
  Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
  Timer2.setPeriod(LED_RATE);
  Timer2.setCompare(TIMER_CH1, 1);
  Timer2.attachInterrupt(TIMER_CH1, timer_);
  serialMode = 1;
}

void loop() {
  stm32_uart1();
  stm32_uart2();
}
