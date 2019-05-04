#include <IRremote.h>

#define FNV_PRIME_32 16777619
#define FNV_BASIS_32 2166136261

decode_results results;

int compare(unsigned int oldval, unsigned int newval) {
  if (newval < oldval * .8) {
    return 0;
  } else if (oldval < newval * .8) {
    return 2;
  } else {
    return 1;
  }
}

unsigned long decodeHash(decode_results *results) {
  unsigned long hash = FNV_BASIS_32;
  for (int i = 1; i + 2 < results->rawlen; i++) {
    int value = compare(results->rawbuf[i], results->rawbuf[i + 2]);
    // Add value into the hash
    hash = (hash * FNV_PRIME_32) ^ value;
  }
  return hash;
}

// switch (hash) {
// case 0x6AF3C2F4:
//   Serial.println("TV");
//   break;
// case 0xD6D52168:
//   Serial.println("TV toggle");
//   break;
// case 0x967BB80C:
//   Serial.println("CH+");
//   break;
// case 0xD1D4CC60:
//   Serial.println("CH+ toggle");
//   break;
// case 0x17B4A228:
//   Serial.println("RADIO");
//   break;
// case 0x530DB67C:
//   Serial.println("RADIO toggle");
//   break;
// case 0x58B59A1D:
// case 0x940EAE71:
//   Serial.println("VOL-");
//   break;
// case 0x46E683E1:
// case 0xF5102315:
//   Serial.println("FULLSCREEN");
//   break;
// case 0x5BB59ED4:
// case 0x970EB328:
//   Serial.println("VOL+");
//   break;
// case 0x1AB4A6E1:
// case 0x560DBB35:
//   Serial.println("MUTE");
//   break;
// case 0x937BB355:
// case 0xCED4C7A9:
//   Serial.println("CH-");
//   break;
// case 0x4025F0FC:
// case 0xEE4F9030:
//   Serial.println("SOURCE");
//   break;
// case 0x56EF334B:
// case 0xC2D091BF:
//   Serial.println("1");
//   digitalWrite(relays_pins[0], !digitalRead(relays_pins[0]));
//   break;
// case 0x17313DCE:
// case 0x528A5222:
//   Serial.println("2");
//   digitalWrite(relays_pins[1], !digitalRead(relays_pins[1]));
//   break;
// case 0xE4400B14:
// case 0x50216988:
//   Serial.println("3");
//   digitalWrite(relays_pins[2], !digitalRead(relays_pins[2]));
//   break;
// case 0x321579D4:
// case 0x6D6E8E28:
//   Serial.println("4");
//   digitalWrite(relays_pins[3], !digitalRead(relays_pins[3]));
//   break;
// case 0x33157B67:
// case 0x6E6E8FBB:
//   Serial.println("5");
//   digitalWrite(relays_pins[4], !digitalRead(relays_pins[4]));
//   break;
// case 0xA41CE90D:
// case 0xDF75FD61:
//   Serial.println("6");
//   digitalWrite(relays_pins[5], !digitalRead(relays_pins[5]));
//   break;
// case 0xFE084450:
// case 0x69E9A2C4:
//   Serial.println("7");
//   digitalWrite(relays_pins[6], !digitalRead(relays_pins[6]));
//   break;
// case 0x1DDBEF8C:
// case 0x593503E0:
//   Serial.println("8");
//   digitalWrite(relays_pins[7], !digitalRead(relays_pins[7]));
//   break;
// case 0x1ADBEAD5:
// case 0x5634FF29:
//   Serial.println("9");
//   break;
// case 0x8B47AD:
// case 0x3BE45C01:
//   Serial.println("RESERVED");
//   break;
// case 0x55EF31B6:
// case 0xC1D0902A:
//   Serial.println("0");
//   break;
// case 0x7BA067FF:
// case 0xCD76C8CB:
//   Serial.println("MINIMIZE");
//   break;
// }
