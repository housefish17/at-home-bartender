   //libraries needed
#include <Wire.h>
#include <HX711.h>
#include <LiquidCrystal_I2C.h>

HX711 scale; //starts scale instance

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

//set pins and calibration factor of scale.
#define calibration_factor 455300.0
#define LOADCELL_DOUT_PIN  6
#define LOADCELL_SCK_PIN  13 //if using NANO uncomment this
//#define LOADCELL_SCK_PIN  52 //if using MEGA uncomment this

const int SMALL_DRINK = 25;
const int MEDIUM_DRINK = 50;
const int LARGE_DRINK = 100;

//button setups
const int buttonPins[] = {2, 3, 4};
int buttonAmount = (sizeof(buttonPins) / sizeof(buttonPins[0]));

//LED as place holder for solenoid
const int ledPin = 9;

void setup() {

  Serial.begin(9600); //serial monitor initialization

  //scale initialization
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  for (int i = 0; i > buttonAmount; i++) {//assigns buttons as inputs
    pinMode(buttonPins[i], INPUT);
  }

  pinMode(ledPin, OUTPUT);

  byte arrow[] = {//creates down arrows for LCD screen
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B11111,
    B01110,
    B00100
  };

  //LCD initialization and greeting
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello, my name is");
  lcd.setCursor(0, 1);
  lcd.print("Lemuel");
  lcd.setCursor(0, 3);
  lcd.createChar(0, arrow);
  delay(2000);

  digitalWrite(ledPin, LOW);

}

void loop() {

  Serial.println(scale.get_units() * 1000);

  bool modeAutomatic = 0;
  bool modeManual = 0;

  lcd.clear();

  do {

    Serial.println(scale.get_units() * 1000);

    lcd.setCursor(0, 0);
    lcd.print("Select a mode");
    lcd.setCursor(0, 2);
    lcd.print("Automatic     Manual");
    lcd.setCursor(2, 3);
    lcd.write(byte(0));
    lcd.setCursor(17, 3);
    lcd.write(byte(0));

    if (digitalRead(buttonPins[0]) == HIGH) {
      modeAutomatic = 1;
    }
    else if (digitalRead(buttonPins[2]) == HIGH) {
      modeManual = 1;
    }

  } while (modeAutomatic == LOW && modeManual == LOW);

  lcd.clear();

  int sizeDrink = 0;

  do {//beginning of mode selction
    if (modeAutomatic == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Automatic Mode");

      sizeDrink = autoQuestions();
    }
    else if (modeManual == 1) {

      lcd.setCursor(0, 0);
      lcd.print("Manual Mode");

      delay(2000);

      lcd.clear();

      do {

        lcd.setCursor(0, 0);
        lcd.print("Select size drink");
        lcd.setCursor(0, 2);
        lcd.print("SMALL  MEDIUM  LARGE");
        lcd.setCursor(2, 3);
        lcd.write(byte(0));
        lcd.setCursor(9, 3);
        lcd.write(byte(0));
        lcd.setCursor(17, 3);
        lcd.write(byte(0));

        if (digitalRead(buttonPins[0]) == HIGH) {
          sizeDrink = SMALL_DRINK;
        }
        else if (digitalRead(buttonPins[1]) == HIGH) {
          sizeDrink = MEDIUM_DRINK;
        }
        else if (digitalRead(buttonPins[2]) == HIGH) {
          sizeDrink = LARGE_DRINK;
        }

      } while (sizeDrink == 0);
    }
  } while (sizeDrink == 0); //end of mode selection

  pourDrink(sizeDrink);

}

void pourDrink(int size) {

  lcd.clear();

  scale.tare();
  delay(3000);
  scale.tare();

  while ((scale.get_units() * 1000) <= size) {

    Serial.println(scale.get_units() * 1000);
    Serial.println(size);

    lcd.setCursor(0, 0);
    lcd.print("Pouring your drink");
    digitalWrite(ledPin, HIGH);
  }

  digitalWrite(ledPin, LOW);

  return;
}

int autoQuestions() {
  delay(1000);
  int answerOne = questionOne();
  delay(1000);
  int answerTwo = questionTwo();
  //int answerThree = questionThree();

  int totalSum = answerOne + answerTwo;// + answerThree;

  return totalSum;
}

int questionOne() {
  lcd.clear();
  int answerOne = 100;

  do {
    lcd.setCursor(0, 0);
    lcd.print("Are you married?");
    lcd.setCursor(0, 2);
    lcd.print(" YES            NO  ");
    lcd.setCursor(2, 3);
    lcd.write(byte(0));
    lcd.setCursor(17, 3);
    lcd.write(byte(0));

    if (digitalRead(buttonPins[0]) == HIGH) {
      answerOne = 10;
    }
    else if (digitalRead(buttonPins[2]) == HIGH) {
      answerOne = 0;
    }
  } while (answerOne == 100);

  return answerOne;
}

int questionTwo() {
    lcd.clear();
    int answerTwo = 100;

  do {
    lcd.setCursor(0, 0);
    lcd.print("Got any kids?");
    lcd.setCursor(0, 2);
    lcd.print(" <=1     2      >=3 ");
    lcd.setCursor(2, 3);
    lcd.write(byte(0));
    lcd.setCursor(9, 3);
    lcd.write(byte(0));
    lcd.setCursor(17, 3);
    lcd.write(byte(0));

    if (digitalRead(buttonPins[0]) == HIGH) {
      answerTwo = 0;
    }
    else if (digitalRead(buttonPins[1]) == HIGH) {
      answerTwo = 5;
    }
    else if(digitalRead(buttonPins[2]) == HIGH){
      answerTwo = 10;
    }
  } while (answerTwo == 100);

  return answerTwo;

}

int questionThree() {

}
