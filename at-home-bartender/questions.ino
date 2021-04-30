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

int Question_five() {
  lcd.clear();
  int answer = 100;

  do {
    lcd.setCursor(0, 0);
    lcd.print("Do you have a final");
    lcd.setCursor(0, 1);
    lcd.print("in a god damn one");
    lcd.setCursor(0, 2);
    lcd.print("credit lab class?");
    lcd.setCursor(1 , 3);
    lcd.print("No           Yes");
    //lcd.setCursor(15, 3);
    //lcd.write("Yes");

    if (digitalRead(buttonPins[0]) == HIGH) {
      answer = 0;
    }
    else if (digitalRead(buttonPins[2]) == HIGH) {
      answer = 10;
    }
  } while (answer == 100);

  return answer;
}
