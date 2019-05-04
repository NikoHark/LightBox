#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>


#define FULL_CLOCK_CYCLE_COUNT 625 // 16000000/100*256 (16Mhz/100Hz*256count)
#define PULSE 4 // trigger pulse width (counts)

uint8_t dimmer_pins[] = {40, 41, 43, 42};

// volatile uint16_t *comparators[] = {&OCR1A, &OCR3A, &OCR4A, &OCR5A};

unsigned int increase(int oldval){
  if(oldval >= 95 || oldval <= 0){
    return 5;
  }
  return oldval+10;
}

//(see ATMEGA 328 data sheet pg 134 for more details)
void initTimer(){
OCR1A = 000;   // initialize the comparator
TIMSK1 = 0x03; // enable comparator A and overflow interrupts
TCCR1A = 0x00; // timer control registers set for
TCCR1B = 0x00; // normal operation, timer disabled

OCR3A = 000;
TIMSK3 = 0x03;
TCCR3A = 0x00;
TCCR3B = 0x00;

OCR4A = 000;
TIMSK4 = 0x03;
TCCR4A = 0x00;
TCCR4B = 0x00;

OCR5A = 000;
TIMSK5 = 0x03;
TCCR5A = 0x00;
TCCR5B = 0x00;
}

// Interrupt Service Routines
void zeroCrossingInterrupt() { // zero cross detect
  TCCR1B = 0x04;               // start timer with divide by 256 input
  TCNT1 = 0;                   // reset timer - count from zero

  TCCR3B = 0x04;
  TCNT3 = 0;

  TCCR4B = 0x04;
  TCNT4 = 0;

  TCCR5B = 0x04;
  TCNT5 = 0;
}

ISR(TIMER1_COMPA_vect) {              // comparator match
  digitalWrite(dimmer_pins[0], HIGH); // set TRIAC gate to high
  TCNT1 = 65536 - PULSE;              // trigger pulse width
}
ISR(TIMER1_OVF_vect) {               // timer1 overflow
  digitalWrite(dimmer_pins[0], LOW); // turn off TRIAC gate
  TCCR1B = 0x00;                     // disable timer stopd unintended triggers
}

ISR(TIMER3_COMPA_vect) {
  digitalWrite(dimmer_pins[1], HIGH);
  TCNT3 = 65536 - PULSE;
}
ISR(TIMER3_OVF_vect) {
  digitalWrite(dimmer_pins[1], LOW);
  TCCR3B = 0x00;
}

ISR(TIMER4_COMPA_vect) {
  digitalWrite(dimmer_pins[2], HIGH);
  TCNT4 = 65536 - PULSE;
}
ISR(TIMER4_OVF_vect) {
  digitalWrite(dimmer_pins[2], LOW);
  TCCR4B = 0x00;
}

ISR(TIMER5_COMPA_vect) {
  digitalWrite(dimmer_pins[3], HIGH);
  TCNT5 = 65536 - PULSE;
}
ISR(TIMER5_OVF_vect) {
  digitalWrite(dimmer_pins[3], LOW);
  TCCR5B = 0x00;
}
