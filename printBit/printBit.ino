int pins[3];
int counter = 0;
boolean countUp = true;


void setup() {
  for (int i = 0; i < 3; i++) {
    //Assign number of pins
    pins[i] = i + 4;
    // initialize the LED pins as an outputs:
    pinMode(pins[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(pins[i], bitRead(counter, i));
    //Serial.println(bitRead(counter, i));
  }
  if (countUp){
    counter++;
  }
  else {
    counter--;
  }
  if (counter == 0 || counter == 7){
    countUp = !countUp;
  }
  Serial.println(counter);
  delay(1000); //wait 1s
}
