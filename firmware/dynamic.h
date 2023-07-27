byte data[8];
bool enableMinutes = true, enableHours = true;
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
  data[0] = hrs / 10;
  data[1] = hrs % 10;
  data[3] = mins / 10;
  data[7] = mins % 10;
}

ISR(TIMER2_COMPA_vect){
  PORTC = ((PORTC & 0b11110000) | (((PORTC & 0b1111) << 1) >> 4) | (((PORTC & 0b1111) << 1) & 0b1111));

  if(!enableHours && ((PORTC & 0b1111) == 0b1110 || (PORTC & 0b1111) == 0b1101)){
    PORTD = 127;
    return;
  }
  if(!enableMinutes && ((PORTC & 0b1111) == 0b1011 || (PORTC & 0b1111) == 0b0111)){
    PORTD = 127;
    return;
  }

  PORTD = ~digits[data[(~PORTC & 0b1111) - 1]]; 0b0001 0b0010 0b0100 0b1000 0b0001
}
