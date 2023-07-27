void checkSegments(){
    data[0] = 127;
    data[1] = 127;
    data[3] = 127;
    data[7] = 127;
    delay(200);
    
    for(uint8_t i = 0; i < 7; i++){
        data[0] = data[0] << 1;
        data[1] = data[0];
        data[3] = data[0];
        data[7] = data[0];
        delay(200);
    }
    for(uint8_t i = 0; i < 4; i++){
        data[0] = 0;
        data[1] = 0;
        data[3] = 0;
        data[7] = 0;
        data[(1 << i) - 1] = 127;
        delay(300);
    }
    
}

void scroll(){
    uint8_t _data[8];
    _data[0] = data[0];
    _data[1] = data[1];
    _data[3] = data[3];
    _data[7] = data[7];

    data[0] = 127;
    data[1] = 127;
    data[3] = 127;
    data[7] = 127;
    delay(100);

    for(uint8_t i = 0; i < 4; i++){
        data[0] = data[1];
        data[1] = data[3];
        data[3] = data[7];
        data[7] = _data[(1 << i) - 1];
        delay(300);
    }
}