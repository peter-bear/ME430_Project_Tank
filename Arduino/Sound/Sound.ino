void setup(){
    pinMode(8, OUTPUT);
    pinMode(6, INPUT_PULLUP);
}

void loop(){

        fire();
    
}

void fire(){
    tone(8, 600, 700); // play tone 72 (C6 = 1047 Hz)

}
