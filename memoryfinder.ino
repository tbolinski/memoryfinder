const int button1 = D0;
const int button2 = D1;
const int button3 = D2;
const int button4 = D3;

const int led1 = D10;
const int led2 = D9;
const int led3 = D8;
const int led4 = D7;

unsigned char maxSequenceLength = 0;
bool doubleGame = false;

void setup() {
  randomSeed(analogRead(0));

  // Init leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // Init buttons
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

  // time in milliseconds after the program has started
  unsigned long startTime = millis();
}

void loop() {
  chooseGamemode();
  delay(500);

  if (!doubleGame) {
    unsigned char sequence[maxSequenceLength];
    unsigned char length = 1;

    while (length <= maxSequenceLength) {
      sequence[length - 1] = random(4) + 1;

      for (int i = 0; i < length; i++) {
        flash(sequence[i]);
        delay(250);
      }

      for (int i = 0; i < length; i++) {
        unsigned char input = getButtonPress(10000);
        if (input != sequence[i]) {
          gameOver();
          return;
        }
      }

      length++;
    }
  }
  else {
    bool side = 0;
    unsigned char sequence[2][maxSequenceLength];
    unsigned char length = 1;

    sequence[1][0] = random(4) + 1;

    while (length <= maxSequenceLength) {
      sequence[side][side + length - 1] = random(4) + 1;

      for (int i = 0; i < length; i++) {
        flash(sequence[side][i+side]);
        delay(250);
      }

      for (int i = 0; i < length; i++) {
        unsigned char input = getButtonPress(10000);
        if (input != sequence[!side][i]) {
          gameOver();
          return;
        }
      }
      if (side == 0){
        side = 1;
      }
      else {
        side = 0;
        length++;
      }
    }
  }
  win();
}

void gameOver() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    delay(200);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    delay(200);
  }
}

void flash(unsigned char num) {
  const int led = getLed(num);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
}

int getLed(unsigned char num){
  if (num == 1) {return led1;}
  if (num == 2) {return led2;}
  if (num == 3) {return led3;}
  if (num == 4) {return led4;}
  return led1;
}

unsigned char getButtonPress(unsigned int time) {
  unsigned long startTime = millis();
  while (true) {
    if ((startTime - millis() <= time) && (time != 0)) {
      if (digitalRead(button1) == LOW) {return 1;}
      if (digitalRead(button2) == LOW) {return 2;}
      if (digitalRead(button3) == LOW) {return 3;}
      if (digitalRead(button4) == LOW) {return 4;}
    }
    else {return 0;}
  }
}

void win() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(led1, HIGH);
    delay(100);
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    delay(100);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    delay(100);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    delay(100);
    digitalWrite(led4, LOW);
  }
}

void chooseGamemode() {
  digitalWrite(getLed(1), HIGH);
  digitalWrite(getLed(2), HIGH);
  digitalWrite(getLed(3), HIGH);
  digitalWrite(getLed(4), HIGH);

  unsigned char button = getButtonPress(0);

  switch (button) {
    case 1:
      maxSequenceLength = 5;
      doubleGame = false;
      break;
    case 2:
      maxSequenceLength = 15;
      doubleGame = false;
      break;
    case 3:
      maxSequenceLength = 5;
      doubleGame = true;
      break;
    case 4:
      maxSequenceLength = 15;
      doubleGame = true;
      break;
  }

  digitalWrite(getLed(1), LOW);
  digitalWrite(getLed(2), LOW);
  digitalWrite(getLed(3), LOW);
  digitalWrite(getLed(4), LOW);

  delay(250);
  digitalWrite(getLed(button), HIGH);
  delay(500);
  digitalWrite(getLed(button), LOW);
  delay(250);
  digitalWrite(getLed(button), HIGH);
  delay(500);
  digitalWrite(getLed(button), LOW);
  delay(250);
  digitalWrite(getLed(button), HIGH);
  delay(500);
  digitalWrite(getLed(button), LOW);
}