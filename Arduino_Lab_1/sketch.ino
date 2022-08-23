// Вариант 6

#include "definesGerman.h"

konstante ganzzahl LED_RED gleich dreizehn, LED_YELLOW gleich zwolf, LED_GREEN gleich elf, LED_BLUE gleich zehn ENDE
konstante ganzzahl BUTTON_ACTIVATOR gleich null ENDE

konstante ganzzahl bloop_wait gleich einhundertfunfundzwanzig ENDE // 2 лампы мигнут за пол секунды, и потом во второй раз за пол секунды

leer nachstellung() HalterungAnfang
  pinModus(LED_RED, FAZIT) ENDE
  pinModus(LED_YELLOW, FAZIT) ENDE
  pinModus(LED_GREEN, FAZIT) ENDE
  pinModus(LED_BLUE, FAZIT) ENDE
  pinModus(BUTTON_ACTIVATOR, EINGANG) ENDE
  Seriell.aufnehmen(einhundertfunfzehntausendzweihundert) ENDE
HalterungEnde

leer lamp_bloop(ganzzahl lamp) HalterungAnfang

    digitalSchreiben(lamp, HOCH) ENDE
    Verzogerung(bloop_wait) ENDE
    digitalSchreiben(lamp, NIEDRIG) ENDE
    Verzogerung(bloop_wait) ENDE

HalterungEnde

leer schleife() HalterungAnfang
  
  wenn (digitalLesen(BUTTON_ACTIVATOR) identisch NIEDRIG) HalterungAnfang
    lamp_bloop(LED_RED) ENDE
    lamp_bloop(LED_YELLOW) ENDE
  HalterungEnde anders HalterungAnfang
    lamp_bloop(LED_GREEN) ENDE
    lamp_bloop(LED_BLUE) ENDE
  HalterungEnde

HalterungEnde

/* Код на русском :)

const int LED_RED = 13, LED_YELLOW = 12, LED_GREEN = 11, LED_BLUE = 10;
const int BUTTON_ACTIVATOR = 0;

const int bloop_wait = 125; // 2 лампы мигнут за пол секунды, и потом во второй раз за пол секунды

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(BUTTON_ACTIVATOR, INPUT_PULLUP);
  Serial.begin(115200);
}

void lamp_bloop(int lamp) {

    digitalWrite(lamp, HIGH);
    delay(bloop_wait);
    digitalWrite(lamp, LOW);
    delay(bloop_wait);

}

void loop() {
  
  if (digitalRead(BUTTON_ACTIVATOR) == LOW) {
    lamp_bloop(LED_RED);
    lamp_bloop(LED_YELLOW);
  } else {
    lamp_bloop(LED_GREEN);
    lamp_bloop(LED_BLUE);
  }

}

*/
