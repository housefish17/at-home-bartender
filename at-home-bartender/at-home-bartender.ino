//libraries needed
#include <Wire.h>
#include <HX711.h>
#include <LiquidCrystal_I2C.h>

HX711 scale; //starts scale object

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

//set pins and calibration factor of scale.
#define calibration_factor 455300.0

#define LOADCELL_DOUT_PIN  6
#define LOADCELL_SCK_PIN  13 //if using NANO uncomment this
//#define LOADCELL_SCK_PIN  52 //if using MEGA uncomment this

//size of drink options (in grams)
const int SMALL_DRINK = 25;
const int MEDIUM_DRINK = 50;
const int LARGE_DRINK = 100;

//button setups
const int buttonPins[] = {2, 3, 4};
int buttonAmount = (sizeof(buttonPins) / sizeof(buttonPins[0]));

//solenoid pin
const int solenoidPin = 9;

void setup() {

  Serial.begin(9600); //serial monitor initialization

  //scale initialization
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  for (int i = 0; i > buttonAmount; i++) {//assigns buttons as inputs
    pinMode(buttonPins[i], INPUT);
  }

  pinMode(solenoidPin, OUTPUT); //set solenoid pin as output

  byte arrow[] = {//creates down arrow for LCD screen
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

  digitalWrite(solenoidPin, LOW); //initialize solenoid pin to low

}

void loop() {

  //Serial.println(scale.get_units() * 1000);

  //two variables to contain which mode selected
  bool modeAutomatic = 0;
  bool modeManual = 0;

  lcd.clear();

  do {

    //Serial.println(scale.get_units() * 1000);

    lcd.setCursor(0, 0);
    lcd.print("Select a mode");
    lcd.setCursor(0, 2);
    lcd.print("Automatic     Manual");
    lcd.setCursor(2, 3);
    lcd.write(byte(0));
    lcd.setCursor(17, 3);
    lcd.write(byte(0));

    //determines button press and subsequently the mode
    if (digitalRead(buttonPins[0]) == HIGH) {
      modeAutomatic = 1;
    }
    else if (digitalRead(buttonPins[2]) == HIGH) {
      modeManual = 1;
    }

    //continues until a mode is selected
  } while (modeAutomatic == LOW && modeManual == LOW);

  lcd.clear();

  int sizeDrink = 0; //initialize drink size to zero
/*
 * 
 */
  do {
    
    if (modeAutomatic == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Automatic Mode");
      
      sizeDrink = autoQuestions(); //begins the question function.
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
  } while (sizeDrink == 0); //end of drink sizing

  pourDrink(sizeDrink);

}

//pourDrink function zeroes out the scale and compares the current
//scale reading to the size passed to it that was determined by the
//various algorithms (either manual or automatic mode).
//While the solenoid pin is energized, the LCD displays "pouring your drink"
void pourDrink(int size) {

  lcd.clear();

  scale.tare();
  delay(1000);
  scale.tare();

  while ((scale.get_units() * 1000) <= size) {

    Serial.println(scale.get_units() * 1000);
    Serial.println(size);

    lcd.setCursor(0, 0);
    lcd.print("Pouring your drink");
    digitalWrite(solenoidPin, HIGH);
  }

  digitalWrite(solenoidPin, LOW);

  return;
}

int autoQuestions() { //questions are exlcusive of their relative answers. They are numbered in order of creation.
  delay(1000);
  int answerOne = questionThree();
  lcd.clear();
  delay(1000);
  int answerTwo = questionOne();
  lcd.clear();
  delay(1000);
  int answerThree = questionTwo();
  lcd.clear();
  delay(1000);
  int answerFour = questionFour();
  delay(1000);
  int totalSum = answerOne + answerTwo + answerThree + answerFour;

  return totalSum;
}

int questionOne() {
  lcd.clear();
  int answer = 100;

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
      answer = 10;
    }
    else if (digitalRead(buttonPins[2]) == HIGH) {
      answer = 0;
    }
  } while (answer == 100);

  return answer;
}

int questionTwo() {
  lcd.clear();
  int answer = 100;

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
      answer = 0;
    }
    else if (digitalRead(buttonPins[1]) == HIGH) {
      answer = 5;
    }
    else if (digitalRead(buttonPins[2]) == HIGH) {
      answer = 10;
    }
  } while (answer == 100);

  return answer;
}

int questionThree() {
  lcd.clear();
  int answer = 100;

  do {
    lcd.setCursor(0, 0);
    lcd.print("How was your day?");
    lcd.setCursor(0, 2);
    lcd.print(" Bad    Meh    Great");
    lcd.setCursor(2, 3);
    lcd.write(byte(0));
    lcd.setCursor(9, 3);
    lcd.write(byte(0));
    lcd.setCursor(17, 3);
    lcd.write(byte(0));

    if (digitalRead(buttonPins[0]) == HIGH) {
      answer = 0;
    }
    else if (digitalRead(buttonPins[1]) == HIGH) {
      answer = 5;
    }
    else if (digitalRead(buttonPins[2]) == HIGH) {
      answer = 10;
    }
  } while (answer == 100);

  return answer;
}

int questionFour() {
  lcd.clear();
  int answer = 100;

  do {
    lcd.setCursor(0, 0);
    lcd.print("Do you have");
    lcd.setCursor(0, 1);
    lcd.print("distinguished taste?");
    lcd.setCursor(0, 2);
    lcd.print(" No       Of Course!");
    lcd.setCursor(2, 3);
    lcd.write(byte(0));
    lcd.setCursor(17, 3);
    lcd.write(byte(0));

    if (digitalRead(buttonPins[0]) == HIGH) {
      answer = 0;
    }
    else if (digitalRead(buttonPins[2]) == HIGH) {
      answer = 10;
    }
  } while (answer == 100);

  return answer;
}
