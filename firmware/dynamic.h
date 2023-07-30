volatile byte data[4];
volatile byte current_dig = 0;
volatile byte disable_dig = 0; 

byte digits[] = {
  0b111111,
  0b1100,
  0b1011011,
  0b1011110,
  0b1101100,
  0b1110110,
  0b1110111,
  0b11100,
  0b1111111,
  0b1111110,
};

void Timer2_init() {
  TCCR2A = 1 << WGM21;
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
  TIMSK2 |= 1 << OCIE2A;
  OCR2A = 15;
}

void sendToDisplay(byte hrs, byte mins){
  data[0] = ~digits[hrs / 10];
  data[1] = ~digits[hrs % 10];
  data[2] = ~digits[mins / 10];
  data[3] = ~digits[mins % 10];
}

ISR(TIMER2_COMPA_vect){  
  PORTC = ~(1 << (current_dig & 0b11));

  if(disable_dig & (1 << (current_dig & 0b11))){
    PORTD = 127;
  }
  else {
    PORTD = data[(current_dig & 0b11)];
  }
  
  // if (current_dig & 0b10000000){
  //   disable_dig |= 1;

  // } else{
  //   disable_dig &= ~1;
  // }
  current_dig++;
}
