void setup(){
    pinMode(8, OUTPUT);
    pinMode(6, INPUT_PULLUP);
}

void loop(){
    if(!digitalRead(6))
        fire();
    
}

void fire(){
    tone(8, 600, 700); // play tone 72 (C6 = 1047 Hz)
    delay(700); // Wait for 300 millisecond(s)
    noTone(8);
    
    
    tone(8, 150, 100); // play tone 72 (C6 = 1047 Hz)
    delay(100); // Wait for 300 millisecond(s)
    noTone(8);
    
    tone(8, 100, 100); // play tone 72 (C6 = 1047 Hz)
    delay(100); // Wait for 300 millisecond(s)
    noTone(8);
}