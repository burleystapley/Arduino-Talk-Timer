#include <LiquidCrystal.h>

const int rs = 12;
const int en = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

const int startSwitchPin = 7;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

unsigned long startTime;
unsigned long elapsedTime;
boolean timerStarted = false;
boolean lastSwitchState = HIGH;
unsigned long messageStartTime;
int messageDuration = 10000; // 2 seconds in milliseconds
boolean displayFirstMessage = true;

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Mic 1 on starts");
  lcd.setCursor(0, 1);
  lcd.print("the timer...");

  pinMode(startSwitchPin, INPUT_PULLUP);

  startTime = 0;
  messageStartTime = millis();
}

void loop() {
  int switchState = digitalRead(startSwitchPin);

  if (switchState == LOW && lastSwitchState == HIGH) {
    if (!timerStarted) {
      timerStarted = true;
      startTime = millis();
      lcd.clear();
    }
  }

  if (switchState == HIGH && lastSwitchState == LOW) {
    if (timerStarted) {
      timerStarted = false;
      startTime = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mic 1 on starts");
      lcd.setCursor(0, 1);
      lcd.print("the timer...");
      messageStartTime = millis(); // Reset the message start time
      displayFirstMessage = true; // Reset to display the first message
    }
  }

  lastSwitchState = switchState;

  if (timerStarted) {
    elapsedTime = millis() - startTime;

    int seconds = (elapsedTime / 1000) % 60;
    int minutes = (elapsedTime / 60000) % 60;

    lcd.setCursor(0, 0);
    lcd.print("Break Length: ");
    lcd.setCursor(6, 1);
    lcd.print(minutes);
    lcd.print(":");
    char secondsStr[3];
    sprintf(secondsStr, "%02d", seconds);
    lcd.print(secondsStr);
  } else {
    // Check if it's time to switch the message ("screen saver" with station slogan)
    if (millis() - messageStartTime > messageDuration) {
      lcd.clear();
      if (displayFirstMessage) {
        lcd.setCursor(0, 0);
        lcd.print("106.3 The Fox");
        lcd.setCursor(0, 1);
        lcd.print("1063TheFox.com");
      } else {
        lcd.setCursor(0, 0);
        lcd.print("Mic on starts");
        lcd.setCursor(0, 1);
        lcd.print("the timer...");
      }
      // Toggle the flag for the next message
      displayFirstMessage = !displayFirstMessage;
      // Reset the message start time
      messageStartTime = millis();
    }
  }
}
