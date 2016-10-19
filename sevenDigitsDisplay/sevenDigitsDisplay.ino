int segmentPins[7];
int counter = 0;
boolean countUp = true;
int number[10][7] = { {1,1,1,1,1,1,0} , {0,1,1,0,0,0,0} , {1,1,0,1,1,0,1} , {1,1,1,1,0,0,1} , {0,1,1,0,0,1,1} , {1,0,1,1,0,1,1} , {1,0,1,1,1,1,1} , {1,1,1,0,0,0,0} , {1,1,1,1,1,1,1} , {1,1,1,0,0,1,1} };
//                  0                 1                 2                 3                 4                 5                 6                 7                 8                 9


void setup() {
  for (int i = 0; i < 7; i++) {
    //Assign number of pins
    segmentPins[i] = i + 4;
    // initialize the LED pins as an outputs:
    pinMode(segmentPins[i], OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  //writeDigit(counter);
  //int temporaryArray[] = number[counter];
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], number[counter][i]);
  }
  if (countUp){
    counter++;
  }
  else {
    counter--;
  }
  if (counter == 0 || counter == 9){
    countUp = !countUp;
  }
  Serial.println(counter);
  delay(1000); //wait 1s
}
/*
void writeDigit(int number) {
  switch (number)
    case (number):
       
    break;

    case 1:

    break;
    case 2:

    break;
    case 3:

    break;
    case 4:

    break;
    case 5:

    break;
    case 6:

    break;
    case 7:

    break;
    case 8:

    break;
    case 9:

    break;
    default:

    break;
}
*/
