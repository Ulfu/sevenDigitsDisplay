const int timer = 500; //
int gameOverTimer;//
int pins[3];
//Set game variables
int counter = 0;
boolean countUp = true;
boolean gameOver = false;
int scorePlayerOne = 0;
int scorePlayerTwo = 0;
int ledSpeed = 800;

boolean onOff = true;
#define onOffPin 7
#define buttonOne 2
#define buttonTwo 3

unsigned long buttonOneClicked = 0;
unsigned long buttonTwoClicked = 0;


void setup() {
  pinMode(buttonOne, INPUT_PULLUP);
  pinMode(buttonTwo, INPUT_PULLUP);
  pinMode(onOffPin, OUTPUT);
  digitalWrite(onOffPin, onOff);
  
  for (int i = 0; i < 3; i++) {
    //Assign number of pins
    pins[i] = i + 4;
    // initialize the LED pins as an outputs:
    pinMode(pins[i], OUTPUT);
  }
  attachInterrupt(digitalPinToInterrupt(buttonOne), hitOne, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonTwo), hitTwo, FALLING);
  Serial.begin(9600);
}

void loop() {
  writeCounter();
  
  if (counter < 0 || counter > 7 && !gameOver) {//If the counter is outside of screen and game is not already set to gameovermode then set to gameovermode
    setGameOver();
  }
  Serial.println(counter);
  
  if (!gameOver) {
    delay(ledSpeed); //wait
    if (countUp){
      counter++;
    }
    else {
      counter--;
    }
  }
  if (gameOver) {
    delay(1000); //wait
    onOff = !onOff;
    digitalWrite(onOffPin, onOff);
  }
}
void hitOne() {
  if (counter == 0 && !countUp && buttonOneClicked - millis() > timer) {
    buttonOneClicked = millis();
    countUp = !countUp;
  }
  resetGame();
}

void hitTwo() {
  if (counter == 7 && countUp && buttonTwoClicked - millis() > timer) {
    buttonTwoClicked = millis();
    countUp = !countUp;
  }
  resetGame();
}
void resetGame() {
  if (gameOverTimer - millis() > 2000 && gameOver) {//Reset game variables
    if (counter >= 7){
      counter = 0;
      countUp = true;
    }
    if (counter =< 0){
      counter = 7;
      countUp = false;
    }
    gameOver = false;
    writeCounter();
    onOff = true;
    digitalWrite(onOffPin, onOff);
  }
}

void setGameOver() {
  ledSpeed *= 0.8;
  if (counter > 7){
    scorePlayerOne ++;
    counter = 7; 
  }
  if (counter < 0){
    scorePlayerTwo ++;
    counter = 0; 
  }
  gameOver = true;
  gameOverTimer = millis();
  onOff = false;
  digitalWrite(onOffPin, onOff);
}

void writeCounter() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(pins[i], bitRead(counter, i));
    //Serial.println(bitRead(counter, i));
  }
}

