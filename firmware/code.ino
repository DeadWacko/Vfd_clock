#include <RTClib.h>
#include "dynamic.h"

RTC_DS3231 rtc;

#define LEFT_BUTTON 8 // поменять на 13 на релизе
#define MIDDLE_BUTTON 12
#define RIGHT_BUTTON 11

uint8_t setMins, setHrs, mode = 0;
uint32_t blinkTime, pressTime, modifyTime, inactiveTime;
bool pressFlg, longPressFlg;

void setup() {
  //Serial.begin(9600);
  rtc.begin();

  GPIO_init();
  Timer2_init();
  initPWM();
  /*data[0] = 1;
  data[1] = 2;
  data[3] = 3;
  data[7] = 4;*/

}

void loop() {
  switch(mode){
    case 0:
      sendToDisplay(rtc.now().hour(), rtc.now().minute());
      break;
    case 1:
      sendToDisplay(setHrs, setMins);
      if(millis() - blinkTime > 500){
        blinkTime = millis();
        enableHours = !enableHours;
      }
      break;
    case 2:
      sendToDisplay(setHrs, setMins);
      if(millis() - blinkTime > 500){
        blinkTime = millis();
        enableMinutes = !enableMinutes;
      }
      break;

  }

  if(digitalRead(MIDDLE_BUTTON)){
    pressTime = millis();
    pressFlg = false;
    longPressFlg = false;
  }

  if(millis() - pressTime > 2000 && !longPressFlg){
    if(mode == 0){
      mode = 1;
      inactiveTime = millis();
      setHrs = rtc.now().hour();
      setMins = rtc.now().minute();
    }
    longPressFlg = true;
  }
  else if(millis() - pressTime > 50 && !pressFlg){
    pressFlg = true;
    if(mode)
      mode++;
    inactiveTime = millis();
    if(mode == 3){
      mode = 0;
      rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), setHrs, setMins, rtc.now().second()));
    }

    enableHours = true;
    enableMinutes = true;
  }

  if(millis() - inactiveTime > 15000 && mode)
    mode = 0;

  if(!digitalRead(LEFT_BUTTON)){
    enableHours = true;
    enableMinutes = true;
    inactiveTime = millis();
    if(millis() - modifyTime > 250){
      modifyTime = millis();
      if(mode == 1)
        setHrs--;
      else if(mode == 2)
        setMins--;

      if(setHrs == 255)
        setHrs = 23;
      if(setMins == 255)
        setMins = 59;
    }
  }

  if(!digitalRead(RIGHT_BUTTON)){
    enableHours = true;
    enableMinutes = true;
    inactiveTime = millis();
    if(millis() - modifyTime > 250){
      modifyTime = millis();
      if(mode == 1)
        setHrs++;
      else if(mode == 2)
        setMins++;

      if(setHrs == 24)
        setHrs = 0;
      if(setMins == 60)
        setMins = 0;
    }
  }
}

void GPIO_init() {
  DDRD |= 0b1111111;
  DDRC |= 0b1111;

  PORTC = 0b1110;

  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(MIDDLE_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
}

void initPWM(){
  DDRB |= _BV(PB1) | _BV(PB2);
  TCCR1A =B10110000;
  TCCR1B =B00010001;
  ICR1 = 320;   //задаем частоту 50КГЦ
  OCR1A = 160 - 2;  //9пине
  OCR1B = 160 + 2 ;  //10пине // настройка портов
}
