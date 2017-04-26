const int timer = 300; //The minimum time beetween every press
unsigned long gameOverTimer;//Save when last game over event occured

#define latchPin 4 //This pin is used to lock leds while shifing out bits to the register
#define clockPin 5 //This pin decides the shiftregisters data clock frequency
#define dataPin 6 //The pin to shift out data to

//Force shift to next shift register by shifting out 16bits


//Set game variables
int counter = 0; //Where is the puck? Saved in this variable
boolean countUp = true;//What direction is the puck moving? Saved in this variable
boolean gameOver = false; //Keeps track if the game is playing or not. If gameOver is false then continue playing
boolean scorePhase = false; //If true show score
int scorePlayerOne = 0;
int scorePlayerTwo = 0;
int ledSpeed = 500; //Puck speed

#define buttonOne 2 //Player one button
#define buttonTwo 3 //Player two button

unsigned long buttonOneClicked = 0;//Saves when was the last confirmed press from player one
unsigned long buttonTwoClicked = 0;//Saves when was the last confirmed press from player two


void setup() {
  pinMode(buttonOne, INPUT_PULLUP);//Setup buttons with pullup
  pinMode(buttonTwo, INPUT_PULLUP);
  pinMode(latchPin, OUTPUT);//Setup pins for the shift register
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //Interupts can even interrupt during the command delay()
  attachInterrupt(digitalPinToInterrupt(buttonOne), hitOne, FALLING);//Listen for Falling (High then Low) on playerOne pin. Then run the hitOne function
  attachInterrupt(digitalPinToInterrupt(buttonTwo), hitTwo, FALLING);//Listen for Falling (High then Low) on playerTwo pin. Then run the hitTwo function
  Serial.begin(9600);//Start serial communication at 9600bit/s. Used for debugging
}

void loop() {
  
  if (counter < 0 || counter > 15 && !gameOver) {//If the counter is outside of screen and game is not already set to gameovermode then set to gameovermode
    setGameOver();
  }
  Serial.println(counter);//Write where the puck is
  
  if (!gameOver) {//If not game over continue the game and move the puck each second
    writeToRegister(prepareForRegister(counter)); //Use the counter to decide which bit should be one. Then write the 2 bytes to the register 
    delay(ledSpeed); //wait. countUp can be changed during this time
    if (countUp){
      counter++; //Move puck up
    }
    else {
      counter--; //Move puck down
    }
  }
  if (gameOver && !scorePhase) {//Blink five times then go to score mode
    if (counter == 15) writeToRegister(prepareScoreForRegister(0, 8)); //Turn on all 8 leds on the losing field
    if (counter == 0) writeToRegister(prepareScoreForRegister(8, 0));
    delay(100); //wait
    writeToRegister(0);//turn all leds off
    delay(100); //wait
    if (millis() - gameOverTimer > 1000) scorePhase = true; //Show the score after one second
  }
  if (gameOver && scorePhase) {//Score mode.Displays the current score
    writeToRegister(prepareScoreForRegister(scorePlayerOne, scorePlayerTwo)); //Convert the score to a binary number where the score is represented by the amount of leds in High state and then shift out the data
  }
}
void hitOne() {
  if (counter == 0 && !countUp && millis() - buttonOneClicked> timer && !gameOver) {//Checks if puck is whithin reach also checks if player is allowed to hit the puck right now
    countUp = !countUp;//Change direction of the puck
    ledSpeed *= 0.8;//Increase puck speed by decreasing the time it waits between each puck movement
  }
  buttonOneClicked = millis();//Save when the press occurred
  resetGame();//If allowed reset the game to continue playing. Score is still saved
}

void hitTwo() {
  if (counter == 15 && countUp && millis() - buttonTwoClicked > timer && !gameOver) {//Checks if puck is whithin reach also checks if player is allowed to hit the puck right now
    countUp = !countUp;//Change direction of the puck
    ledSpeed *= 0.8;//Increase puck speed by decreasing the time it waits between each puck movement
  }
  buttonTwoClicked = millis();//Save when the press occurred
  resetGame();//If allowed reset the game to continue playing. Score is still saved
}
void resetGame() {
  if (millis() - gameOverTimer > 2000 && gameOver && scorePhase) {//if allowed reset all game variables except the score
    if (counter >= 15){//Decide who serves. Player one
      counter = 0;
      countUp = true;
    }
    else if (counter <= 0){//Decide who serves. Player two
      counter = 15;
      countUp = false;
    }
    ledSpeed = 500;//Reset ledSpeed
    scorePhase = false;//Do not show score while the game is on
    gameOver = false;//Continue the game
    writeToRegister(prepareForRegister(counter));//Shift out puck position now for instant response
  }
}

void setGameOver() {
  //ledSpeed = 500; This is now done in the reset function
  if (counter > 15){//Check who scored
    scorePlayerOne ++;//Add a point to player one
    counter = 15; //Keep track off who scored
  }
  if (counter < 0){//Check who scored
    scorePlayerTwo ++;//Add a point to player Two
    counter = 0; //Keep track off who scored
  }
  gameOver = true; //Stop playing
  gameOverTimer = millis();//Saves when goal occurred
  tone(11, 50, 1000); //Play a tone on pin 11 for 1s
}

unsigned int prepareForRegister(int pos){//Returns a variable that has a 1 in the position mentioned in pos.
   unsigned int raised = power(2,pos);
   //Serial.println(pos);
   //Serial.println(raised,2);
   return raised;
}

unsigned int prepareScoreForRegister(int score1, int score2){//Put both scores in the same variable
   unsigned int raised = power(2 , score2) - 1; //This will make the amount of ones equal to the score
   if (raised > 0) while(raised < 32768) {//move the score until it has reached the 16th bit. This is only possible if more than zero was written to the variable.
      raised = raised << 1;
   }
   raised += power(2 , score1) - 1; // add the score from player one
   Serial.println(raised,2); // Print the binary number in binary to serial
   return raised;
}

unsigned int power(int base, int exponent){//Used as the pow() function except this one only works with integers and also returns a int.
  if (exponent == 0) {//define position zero in this recursiv function
    base = 1;
  }
  else {//define the recursiv expression
    base = base * power(base, exponent - 1);
  }
  return base;
}

void writeToRegister(unsigned int number) {//Shifts out the last two bytes to the shift registers
  
    digitalWrite(latchPin, LOW);   //Lock the outputs on the register
    shiftOut(dataPin, clockPin, MSBFIRST, number >> 8); //Write 
    shiftOut(dataPin, clockPin, MSBFIRST, number);  
    digitalWrite(latchPin, HIGH);  //Unlock the outputs on the register
  /*for (int i = 0; i < 3; i++) {
    digitalWrite(pins[i], bitRead(counter, i));
    //Serial.println(bitRead(counter, i));
    */
}

