/*
   режимы:
  "Led_OFF",         //лента выключена
  "RGB_Ring",        //кольцо выбора цвета
  "White",           //белый
  "Red",             //красный
  "Green",           //зелёный
  "Blue",            //синий
  "Yellow",          //жёлтый
  "Azure",           //голубой
  "violet"           //фиолетовый
*/
#define kol_size 8                    //количество режимов -1
#define CLK 4
#define DT 3
#include "GyverEncoder.h"
Encoder enc1(CLK, DT);
#define Led_Red 11
#define Led_Green 10
#define Led_Blue 9
#define button 5
uint8_t r_pwm = 0, g_pwm = 0, b_pwm = 0;

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
  pinMode(Led_Red, OUTPUT);
  pinMode(Led_Green, OUTPUT);
  pinMode(Led_Blue, OUTPUT);
}

void loop() {
  static uint8_t value;
  static uint8_t mode;
  if (button_click(button))(mode == kol_size) ? mode = 0 : mode++;
  enc1.tick();
  if (enc1.isRight())  value++;        // если был поворот направо, увеличиваем на 1
  if (enc1.isLeft())   value--;        // если был поворот налево, уменьшаем на 1
  if (enc1.isFastR()) value += 5;     // если был быстрый поворот направо, увеличиваем на 10
  if (enc1.isFastL()) value -= 5;     // если был быстрый поворот налево, уменьшаем на 10
  switch (mode) {
    case 0:
      //      Serial.println("Led_OFF");
      digitalWrite(Led_Red, HIGH);
      digitalWrite(Led_Green, HIGH);
      digitalWrite(Led_Blue, HIGH);
      break;
    case 1:
      //      Serial.println("RGB_Ring");
      makeColor(value);
      analogWrite(Led_Red, r_pwm);                  // Генерируем ШИМ сигналы
      analogWrite(Led_Green, g_pwm);
      analogWrite(Led_Blue, b_pwm);
      break;
    case 2:
      //      Serial.println("White");
      analogWrite(Led_Red, value);
      analogWrite(Led_Green, value);
      analogWrite(Led_Blue, value);
      break;
    case 3:
      //      Serial.println("Red");
      analogWrite(Led_Red, value);
      digitalWrite(Led_Green, HIGH);
      digitalWrite(Led_Blue, HIGH);
      break;
      break;
    case 4:
      //      Serial.println("Green");
      digitalWrite(Led_Red, HIGH);
      analogWrite(Led_Green, value);
      digitalWrite(Led_Blue, HIGH);
      break;
    case 5:
      //      Serial.println("Blue");
      digitalWrite(Led_Red, HIGH);
      digitalWrite(Led_Green, HIGH);
      analogWrite(Led_Blue, value);
      break;
    case 6:
      //      Serial.println("Yellow");
      analogWrite(Led_Red, value);
      digitalWrite(Led_Blue, HIGH);
      analogWrite(Led_Green, value);
      break;
    case 7:
      //      Serial.println("Azure");
      digitalWrite(Led_Red, HIGH);
      analogWrite(Led_Blue, value);
      analogWrite(Led_Green, value);
      break;
    case 8:
      //      Serial.println("violet");
      analogWrite(Led_Red, value);
      analogWrite(Led_Blue, value);
      digitalWrite(Led_Green, HIGH);
      break;
  }

}


bool button_click(uint8_t pin) {
  static bool flag;
  static uint32_t btnTimer;
  // читаем инвертированное значение для удобства
  bool btnState = !digitalRead(pin);
  bool out;
  if (btnState && !flag && millis() - btnTimer > 100) {
    flag = true;
    btnTimer = millis();
    out = 1;
    //Serial.println("press");
  }
  if (!btnState && flag && millis() - btnTimer > 100) {
    flag = false;
    btnTimer = millis();
    out = 0;
    //Serial.println("release");

  }
  return !out;
}


void makeColor(uint8_t color) {                   // 8-ми битное цветовое колесо
  uint8_t shift = 0;                              // Цветовой сдвиг
  if (color > 170) {                              // Синий - фиолетовый - красный
    shift = (color - 170) * 3;                    // Получаем цветовой сдвиг 0 - 255
    r_pwm = shift, g_pwm = 0, b_pwm = ~shift;     // Получаем компоненты RGB
  } else if (color > 85) {                        // Зеленый - голубой - синий
    shift = (color - 85) * 3;                     // Получаем цветовой сдвиг 0 - 255
    r_pwm = 0, g_pwm = ~shift, b_pwm = shift;     // Получаем компоненты RGB
  } else {                                        // Красный - оранжевый - зеленый
    shift = (color - 0) * 3;                      // Получаем цветовой сдвиг 0 - 255
    r_pwm = ~shift, g_pwm = shift, b_pwm = 0;     // Получаем компоненты RGB
  }
  r_pwm = ((uint16_t)r_pwm  * (255 + 1)) >> 8;   // Применяем яркость
  g_pwm = ((uint16_t)g_pwm  * (255 + 1)) >> 8;   // Применяем яркость
  b_pwm = ((uint16_t)b_pwm  * (255 + 1)) >> 8;   // Применяем яркость
}
