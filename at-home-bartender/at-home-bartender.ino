/*
   The at-home bar tender is a pandemic proof bartender replacement solution.
*/

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
#define LOADCELL_SCK_PIN  13 //uncomment this if using NANO
//#define LOADCELL_SCK_PIN  52 //uncomment this if using MEGA

//size of drink options (in grams)
const int SMALL_DRINK = 25;
const int MEDIUM_DRINK = 50;
const int LARGE_DRINK = 100;

//button setups
const int buttonPins[] = {2, 3, 4};
int buttonAmount = (sizeof(buttonPins) / sizeof(buttonPins[0])); //calculates the number of buttons

//solenoid pin
const int solenoidPin = 9;

//arrays for each of the LCD lines in case formatting is needed
char line_one[21];
char line_two[21];
char line_three[21];
char line_four[21];

void setup() {
  Serial.begin(9600); //opens serial monitor

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

  //two variables to contain selected mode
  bool modeAutomatic = 0;
  bool modeManual = 0;

  //holds final weight of glass and liquid after pour
  double final_weight = 0;

  lcd.clear();

  do {

    //prompts user for mode selection, automatic or manual
    delay(1000);
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

  do
  {
    if (modeAutomatic == 1) { //feeds back to user they selected auto mode
      lcd.setCursor(0, 0);
      lcd.print("Automatic Mode");

      sizeDrink = autoQuestions(); //begins the question function.
    }

    else if (modeManual == 1) { //feeds back to user they selected manual mode

      lcd.setCursor(0, 0);
      lcd.print("Manual Mode");
      delay(2000);
      lcd.clear();

      do {  //prompts user to select a drink size (remember this is manual mode

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

        //determines which button pressed by user, selects drink size as result
        if (digitalRead(buttonPins[0]) == HIGH) {
          sizeDrink = SMALL_DRINK;
        }
        else if (digitalRead(buttonPins[1]) == HIGH) {
          sizeDrink = MEDIUM_DRINK;
        }
        else if (digitalRead(buttonPins[2]) == HIGH) {
          sizeDrink = LARGE_DRINK;
        }
      //loop continues while size of drink doesn't change
      } while (sizeDrink == 0);
    }
  } while (sizeDrink == 0); //end of drink sizing

  pourDrink(sizeDrink); //starts the pour function, passes size to function

  //after drink pour is completed, Lemuel prompts user to enjoy drink
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enjoy your drink!");
  delay(3000);
  lcd.clear();

  do {  //a less jarring way to return to the beginning of the program
    lcd.setCursor(0, 0);
    lcd.print("Press any key...");
  } while (digitalRead(buttonPins[0]) == LOW && digitalRead(buttonPins[1]) == LOW
           && digitalRead(buttonPins[2]) == LOW);
}

//pourDrink function zeroes out the scale and compares the current
//scale reading to the size passed to it that was determined by the
//various algorithms (either manual or automatic mode).
//While the solenoid pin is energized, the LCD displays "pouring your drink"
void pourDrink(int size)
{
  //holds the percentage of the pour completed
  unsigned int percent_complete;

  //stores string of drink size
  String drink_size;

  //determines word for drink size based on size passed to function
  //this is used to feed back to the user what size drink they are
  //receiving. 
  if(size == 25){
    drink_size = String("small");
  }
  else if(size == 50){
    drink_size = String("medium");
  }
  else{
    drink_size = String("large");
  }

  lcd.clear();

  //zeros the scale 
  delay(1000);
  scale.tare();

  //loop runs while current weight is less than selected size
  while ((scale.get_units() * 1000) <= size)
  {
    //shows user how far along the pour is
    percent_complete = ((scale.get_units() * 1000) / size) * 100;

    //sends string to line_four buffer, allows better formatting without
    //having to constantly clear screen. This is useful for displaying
    //continuously updating information, such as percentage complete
    sprintf(line_four, "Progress: %02d%%", percent_complete);

    lcd.setCursor(0, 0);
    lcd.print("Pouring you a");
    lcd.setCursor(0,1);
    lcd.print(drink_size + " drink");
    lcd.setCursor(0, 3);
    lcd.print(line_four); //prints what is stored in line_four buffer
    digitalWrite(solenoidPin, HIGH); //operates the solenoid open
  }

  digitalWrite(solenoidPin, LOW); //closes the solenoid once pour is complete

  return;
}

/*
 * 
 */
int autoQuestions() { //questions are exclusive of their relative answers. They are numbered in order of creation.
  delay(1000);

  int answerOne = questionThree();  //asks question
  lcd.clear();
  delay(1000);

  int answerTwo = questionOne();  //asks another question
  lcd.clear();
  delay(1000);

  int answerThree = questionTwo();  //asks another question
  lcd.clear();
  delay(1000);

  int answerFour = questionFour();  //asks another question
  delay(1000);
  lcd.clear();

  int answer_five = Question_five();  //asks another question
  delay(1000);
  lcd.clear();

  //tallies all the responses
  int totalSum = answerOne + answerTwo + answerThree + answerFour;

  int size_drink; //holds the data to be passed from function

  //if total sum is <= 15, the program locks in a small drink
  if (totalSum <= 15) {
    size_drink = SMALL_DRINK;
  }

  //otherwise if the sum is < 35, the program locks in a medium drink
  else if (totalSum < 35) {
    size_drink = MEDIUM_DRINK;
  }

  //and all other cases the program locks in a large drink.
  else {
    size_drink = LARGE_DRINK;
  }

  return size_drink;      //returns drink size to function caller
}
