#include "dimmer.h"
#include "utils.h"

#include <IRremote.h>

#define ZEROCROSS_PIN 2
#define IR_DIODE_PIN 7

uint8_t relays_pins[] = {52, 53, 51, 50, 49, 48, 47, 46};

IRrecv irrecv(IR_DIODE_PIN);

int dimmer_procentage[] = {0, 0, 0, 0};

void onInitShowcase() {
  Serial.println("Starting Showcase");
  uint16_t state;
  for (size_t i = 1; i < 20; i++) {
    state = (uint16_t)((100 - i * 5) / 100. * FULL_CLOCK_CYCLE_COUNT);
    OCR1A = state;
    OCR3A = state;
    delay(100);
  }
  for (size_t i = 1; i < 10; i++) {
    state = (uint16_t)((i * 10) / 100. * FULL_CLOCK_CYCLE_COUNT);
    OCR1A = state;
    OCR3A = state;
    delay(70);
  }

  for (size_t i = 1; i < 20; i++) {
    state = (uint16_t)((100 - i * 5) / 100. * FULL_CLOCK_CYCLE_COUNT);
    OCR4A = state;
    OCR5A = state;
    delay(100);
  }
  for (size_t i = 1; i < 10; i++) {
    state = (uint16_t)((i * 10) / 100. * FULL_CLOCK_CYCLE_COUNT);
    OCR4A = state;
    OCR5A = state;
    delay(70);
  }
}

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();

  pinMode(ZEROCROSS_PIN, INPUT);     // zero cross detect
  digitalWrite(ZEROCROSS_PIN, HIGH); // enable pull-up resistor

  for (size_t i = 0; i < sizeof(dimmer_pins); i++) {
    pinMode(dimmer_pins[i], OUTPUT); // TRIAC gate control
  }
  initTimer();

  attachInterrupt(0, zeroCrossingInterrupt, RISING);

  for (size_t i = 0; i < sizeof(relays_pins); i++) {
    pinMode(relays_pins[i], OUTPUT);
    digitalWrite(relays_pins[i], HIGH);
  }

  onInitShowcase();
}

// void loop() {
//   if (irrecv.decode(&results)) {
//     Serial.print("'real' decode: ");
//     Serial.print(results.value, HEX);
//     Serial.print(", hash decode: ");
//     Serial.println(decodeHash(&results), HEX);
//     irrecv.resume(); // Resume decoding (necessary!)
//   }
// }

void loop() {
  if (irrecv.decode(&results)) {
    unsigned long hash = decodeHash(&results);
    if (hash == 0x6AF3C2F4 || hash == 0xD6D52168) { // TV
      Serial.println("TV");
    } else if (hash == 0x967BB80C || hash == 0xD1D4CC60) { // CH+
      Serial.println("CH+");
    } else if (hash == 0x17B4A228 || hash == 0x530DB67C) { // RADIO
      Serial.println("RADIO");
    } else if (hash == 0x58B59A1D || hash == 0x940EAE71) { // VOL-
      Serial.println("VOL-");
    } else if (hash == 0x46E683E1 || hash == 0xF5102315) { // FULLSCREEN
      // Serial.println("FULLSCREEN");
      // everthing on
      for (size_t i = 0; i < sizeof(relays_pins); i++) {
        digitalWrite(relays_pins[i], LOW);
      }
      for (size_t i = 0; i < sizeof(dimmer_pins); i++) {
        dimmer_procentage[i] = 5;
      }
      OCR1A = (100 - dimmer_procentage[0]) / 100. * FULL_CLOCK_CYCLE_COUNT;
      OCR3A = (100 - dimmer_procentage[1]) / 100. * FULL_CLOCK_CYCLE_COUNT;
      OCR4A = (100 - dimmer_procentage[2]) / 100. * FULL_CLOCK_CYCLE_COUNT;
      OCR5A = (100 - dimmer_procentage[3]) / 100. * FULL_CLOCK_CYCLE_COUNT;
    } else if (hash == 0x5BB59ED4 || hash == 0x970EB328) { // VOL+
      Serial.println("VOL+");
    } else if (hash == 0x1AB4A6E1 || hash == 0x560DBB35) { // MUTE
      // Serial.println("MUTE");
      // everthing off
      for (size_t i = 0; i < sizeof(relays_pins); i++) {
        digitalWrite(relays_pins[i], HIGH);
      }
      for (size_t i = 0; i < sizeof(dimmer_pins); i++) {
        dimmer_procentage[i] = 95;
      }
      OCR1A = (100 - dimmer_procentage[0]) / 100. * FULL_CLOCK_CYCLE_COUNT;
      OCR3A = (100 - dimmer_procentage[1]) / 100. * FULL_CLOCK_CYCLE_COUNT;
      OCR4A = (100 - dimmer_procentage[2]) / 100. * FULL_CLOCK_CYCLE_COUNT;
      OCR5A = (100 - dimmer_procentage[3]) / 100. * FULL_CLOCK_CYCLE_COUNT;
    } else if (hash == 0x937BB355 || hash == 0xCED4C7A9) { // CH-
      Serial.println("CH-");
    } else if (hash == 0x4025F0FC || hash == 0xEE4F9030) { // SOURCE
      Serial.println("SOURCE");
      // relay toggle buttons
    } else if (hash == 0x56EF334B || hash == 0xC2D091BF) { // 1
      // Serial.println("1");
      digitalWrite(relays_pins[0], !digitalRead(relays_pins[0]));
    } else if (hash == 0x17313DCE || hash == 0x528A5222) { // 2
      // Serial.println("2");
      digitalWrite(relays_pins[1], !digitalRead(relays_pins[1]));
    } else if (hash == 0xE4400B14 || hash == 0x50216988) { // 3
      // Serial.println("3");
      digitalWrite(relays_pins[2], !digitalRead(relays_pins[2]));
    } else if (hash == 0x321579D4 || hash == 0x6D6E8E28) { // 4
      // Serial.println("4");
      digitalWrite(relays_pins[3], !digitalRead(relays_pins[3]));
    } else if (hash == 0x33157B67 || hash == 0x6E6E8FBB) { // 5
      // Serial.println("5");
      digitalWrite(relays_pins[4], !digitalRead(relays_pins[4]));
    } else if (hash == 0xA41CE90D || hash == 0xDF75FD61) { // 6
      // Serial.println("6");
      digitalWrite(relays_pins[5], !digitalRead(relays_pins[5]));
    } else if (hash == 0xFE084450 || hash == 0x69E9A2C4) { // 7
      // Serial.println("7");
      digitalWrite(relays_pins[6], !digitalRead(relays_pins[6]));
    } else if (hash == 0x1DDBEF8C || hash == 0x593503E0) { // 8
      // Serial.println("8");
      digitalWrite(relays_pins[7], !digitalRead(relays_pins[7]));
      // dimmer increase buttons
    } else if (hash == 0x1ADBEAD5 || hash == 0x5634FF29) { // 9
      // Serial.println("9");
      dimmer_procentage[0] = increase(dimmer_procentage[0]);
      OCR1A = (100 - dimmer_procentage[0]) / 100. * FULL_CLOCK_CYCLE_COUNT;
      Serial.print("dimmer0 power: ");
      Serial.println(dimmer_procentage[0]);
    } else if (hash == 0x8B47AD || hash == 0x3BE45C01) { // RESERVED
      // Serial.println("RESERVED");
      dimmer_procentage[1] = increase(dimmer_procentage[1]);
      OCR3A = (100 - dimmer_procentage[1]) / 100. * FULL_CLOCK_CYCLE_COUNT;
      Serial.print("dimmer1 power: ");
      Serial.println(dimmer_procentage[1]);
    } else if (hash == 0x55EF31B6 || hash == 0xC1D0902A) { // 0
      // Serial.println("0");
      dimmer_procentage[2] = increase(dimmer_procentage[2]);
      OCR4A = (100 - dimmer_procentage[2]) / 100. * FULL_CLOCK_CYCLE_COUNT;
      Serial.print("dimmer2 power: ");
      Serial.println(dimmer_procentage[2]);
    } else if (hash == 0x7BA067FF || hash == 0xCD76C8CB) { // MINIMIZE
      // Serial.println("MINIMIZE");
      dimmer_procentage[3] = increase(dimmer_procentage[3]);
      OCR5A = (100 - dimmer_procentage[3]) / 100. * FULL_CLOCK_CYCLE_COUNT;
      Serial.print("dimmer3 power: ");
      Serial.println(dimmer_procentage[3]);
    }
    delay(60);
    irrecv.resume();
  }
}
