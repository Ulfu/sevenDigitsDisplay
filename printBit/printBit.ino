const int timer = 500; //The minimum time beetween every press
unsigned long gameOverTimer;//Save when last game over event occured

#define latchPin 4
#define clockPin 5
#define dataPin 6

//Force shift to next shift register
//#define latchPin1 7 
//#define clockPin1 8
//#define dataPin 9

//Set game variables
int counter = 0;
boolean countUp = true;
boolean gameOver = false;
boolean scorePhase = false;
int scorePlayerOne = 0;
int scorePlayerTwo = 0;
int ledSpeed = 600;

//boolean onOff = true;
//#define onOffPin 7
#define buttonOne 2
#define buttonTwo 3

unsigned long buttonOneClicked = 0;
unsigned long buttonTwoClicked = 0;


void setup() {
  pinMode(buttonOne, INPUT_PULLUP);
  pinMode(buttonTwo, INPUT_PULLUP);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  //pinMode(onOffPin, OUTPUT);
  //digitalWrite(onOffPin, onOff);
  
  attachInterrupt(digitalPinToInterrupt(buttonOne), hitOne, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonTwo), hitTwo, FALLING);
  Serial.begin(9600);
}

void loop() {
  
  if (counter < 0 || counter > 15 && !gameOver) {//If the counter is outside of screen and game is not already set to gameovermode then set to gameovermode
    setGameOver();
  }
  Serial.println(counter);
  
  if (!gameOver) {//If not game over continue the game
    writeToRegister(prepareForRegister(counter));
    //onOff = true;
    //digitalWrite(onOffPin, onOff);
    delay(ledSpeed); //wait
    if (countUp){
      counter++;
    }
    else {
      counter--;
    }
  }
  if (gameOver && !scorePhase) {
    //delay(300); //wait
    //onOff = !onOff;
    //if (gameOver) digitalWrite(onOffPin, onOff);
    if (millis() - gameOverTimer > 1000) scorePhase = true;
  }
  if (gameOver && scorePhase) {
    writeToRegister(prepareScoreForRegister(scorePlayerOne, scorePlayerTwo));
  }
}
void hitOne() {
  if (counter == 0 && !countUp && millis() - buttonOneClicked> timer && !gameOver) {
    buttonOneClicked = millis();
    countUp = !countUp;
    ledSpeed *= 0.8;
  }
  resetGame();
}

void hitTwo() {
  if (counter == 15 && countUp && millis() - buttonTwoClicked > timer && !gameOver) {
    buttonTwoClicked = millis();
    countUp = !countUp;
    ledSpeed *= 0.8;
  }
  resetGame();
}
void resetGame() {
  if (millis() - gameOverTimer > 2000 && gameOver && scorePhase) {//Reset game variables
    if (counter >= 15){
      counter = 0;
      countUp = true;
    }
    else if (counter <= 0){
      counter = 15;
      countUp = false;
    }
    scorePhase = false;
    gameOver = false;
    writeToRegister(prepareForRegister(counter));
    //onOff = true;
    //digitalWrite(onOffPin, onOff);
  }
}

void setGameOver() {
  ledSpeed = 800;
  if (counter > 15){
    scorePlayerOne ++;
    counter = 15; 
  }
  if (counter < 0){
    scorePlayerTwo ++;
    counter = 0; 
  }
  gameOver = true;
  gameOverTimer = millis();
  //onOff = false;
  //digitalWrite(onOffPin, onOff);
}

unsigned int prepareForRegister(int pos){
   unsigned int raised = power(2,pos);
   //Serial.println(pos);
   //Serial.println(raised,2);
   return raised;
}

unsigned int prepareScoreForRegister(int score1, int score2){//Put both scores in the same variable
   unsigned int raised = power(2 , score2) - 1;
   if (raised > 0) while(raised < 32768) {
      raised = raised << 1;
   }
   raised += power(2 , score1) - 1;
   Serial.println(raised,2);
   return raised;
}

unsigned int power(int base, int exponent){
  if (exponent == 0) {
    base = 1;
  }
  else {
    base = base * power(base, exponent - 1);
  }
  return base;
}

void writeToRegister(unsigned int number) {
  
    digitalWrite(latchPin, LOW);   //Lock the outputs on the register
    shiftOut(dataPin, clockPin, MSBFIRST, number >> 8); //Write 
    shiftOut(dataPin, clockPin, MSBFIRST, number);  
    digitalWrite(latchPin, HIGH);  //Unlock the outputs on the register
  /*for (int i = 0; i < 3; i++) {
    digitalWrite(pins[i], bitRead(counter, i));
    //Serial.println(bitRead(counter, i));
    */
}

