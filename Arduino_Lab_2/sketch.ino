/* Немного доработал идею, поэтому сделал 4 светодиода на 4
стадии громкости (1024 / 4) т.е. каждый сектор по 25% и добавил также пищалку для
звука. На мониторчике все выводится и.т.д.
*/

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define LED_BLUE            13
#define LED_GREEN           12
#define LED_YELLOW          11
#define LED_ORANGE          9
#define LED_RED             10

#define BUZZER              8

#define ANALOG_POTENT       A0

int potentiometerValue = 0, potentiometerValuePercantage = 0, prevValue = -1;

void setup() {

  lcd.init();

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  Serial.begin(115200);

}

void lamp_bloop(int lamp_pin) {

  digitalWrite(lamp_pin, HIGH);
  digitalWrite(lamp_pin, LOW);

}

void loop() {

  potentiometerValue = analogRead(ANALOG_POTENT);

  if (prevValue != potentiometerValue) {
    
    potentiometerValuePercantage = ((potentiometerValue / 10) * 100) / (1023 / 10);

    // Из-за ограничения int на 32768 нельзя нормально посчитать проценты, умножая на 100
    // оно просто умирает из-за огромных чисел, так что поделил все на 10, проценты все равно
    // правильно считает, но немного костыль конечно да
    
    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.print("INT: ");
    lcd.print(potentiometerValue);
    lcd.print(" / 1023");
    lcd.setCursor(1, 2);
    lcd.print("PER: ");
    lcd.print(potentiometerValuePercantage);
    lcd.print("% / 100%");
    lcd.setCursor(1, 3);
    prevValue = potentiometerValue;

  }

  if (potentiometerValue == 0) {

    tone(BUZZER, -1);
    lamp_bloop(LED_BLUE);

  } else {

    if (potentiometerValue < 256) {

      tone(BUZZER, 50);
      lamp_bloop(LED_GREEN);

    } else if (potentiometerValue >= 256 && potentiometerValue < 512) {

      tone(BUZZER, 150);
      lamp_bloop(LED_YELLOW);

    } else if (potentiometerValue >= 512 && potentiometerValue < 768) {

      tone(BUZZER, 250);
      lamp_bloop(LED_ORANGE);

    } else if (potentiometerValue >= 768 && potentiometerValue < 1023) {

      tone(BUZZER, 500);
      lamp_bloop(LED_RED);

    } else if (potentiometerValue == 1023) {

      tone(BUZZER, 750);
      lamp_bloop(LED_BLUE);
      lamp_bloop(LED_GREEN);
      lamp_bloop(LED_YELLOW);
      lamp_bloop(LED_ORANGE);
      lamp_bloop(LED_RED);

    }

  }

}
