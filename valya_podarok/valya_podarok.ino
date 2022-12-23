/*
   режимы:
  "Led_OFF",         //лента выключена
  "RGB_Ring",        //кольцо выбора цвета
  "RGB_auto_Ring",   //переливающееся кольцо
  "White",           //белый
  "Red",             //красный
  "Green",           //зелёный
  "Blue",            //синий
  "Yellow",          //жёлтый
  "Azure",           //голубой
  "violet"           //фиолетовый
*/
#define kol_size 9                    //количество режимов -1
#define CLK 4
#define DT 3
#define SW 2
#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);
#define button 5
#define Led_Red 11
#define Led_Green 10
#define Led_Blue 9

void setup() {
  Serial.begin(9600);
  enc1.setType(TYPE1);        // тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип\=
  enc1.setFastTimeout(40);    // таймаут на скорость isFastR. По умолч. 50
  // Пины D9 и D10 - 62.5 кГц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00001001;  // x1 fast pwm
  // Пины D3 и D11 - 62.5 кГц
  TCCR2B = 0b00000001;  // x1
  TCCR2A = 0b00000011;  // fast pwm
  pinMode(button, INPUT_PULLUP);
}

void loop() {
  uint8_t mode;
  uint8_t brightness = 255;
  if(digitalRead(button)==0) mode++;
  if(mode>kol_size) mode = 0;
  Serial.println(mode);
  /*
  switch (mode) {
    case 0:
      Serial.println("Led_OFF");
      break;
    case 1:
      Serial.println("RGB_Ring");
      break;
    case 2:
      Serial.println("RGB_auto_Ring");
      break;
    case 3:
      Serial.println("White");
      break;
    case 4:
      Serial.println("Red");
      break;
    case 5:
      Serial.println("Green");
      break;
    case 6:
      Serial.println("Blue");
      break;
    case 7:
      Serial.println("Yellow");
      break;
    case 8:
      Serial.println("Azure");
      break;
    case 9:
      Serial.println("violet");
      break;
  }
  */
}
