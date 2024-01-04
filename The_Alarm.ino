#include <Wire.h>
#include <EEPROM.h>
#include <RTClib.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// Constants for pin assignments and settings
const int SERVO_PIN = 9;
const int DAY_BUTTON_PIN = 8;
const int BUZZER_PIN = 13;
const int INC_BUTTON_PIN = 11;
const int NEXT_BUTTON_PIN = 10;
const int SET_ALARM_BUTTON_PIN = 12;
const int EEPROM_START_ADDRESS = 11;
const int REFILL_ANGLE = 0;
const int ANGLE_PER_DAY = 25; // Change this if the servo angle per day is different

// Creating objects for LCD and RTC
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); 
RTC_DS1307 rtc;
Servo servoTest;

// Variables for storing current time and alarm settings
int currentHour, currentMinute, currentSecond;
int alarmHour, alarmMinute;
int currentDate = 0;

void setup() {
  initializeComponents();
  // Set RTC to current date and time at first run
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
  DateTime now = rtc.now();
  updateCurrentTime(now);
  displayTimeAndDate(now);
  checkAndSetAlarm();
  checkDayButton();
  operateServo();
  delay(200); // Short delay for loop stability
}

// Initializes all components (LCD, RTC, buttons, servo)
void initializeComponents() {
  Wire.begin();
  rtc.begin();
  lcd.begin(16, 2);
  pinMode(INC_BUTTON_PIN, INPUT_PULLUP);
  pinMode(NEXT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SET_ALARM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(DAY_BUTTON_PIN, INPUT_PULLUP);
  servoTest.attach(SERVO_PIN);
  displayStartupMessage();
}

// Displays a startup message on the LCD
void displayStartupMessage() {
  lcd.setCursor(0, 0);
  lcd.print("Real Time Clock");
  lcd.setCursor(0, 1);
  lcd.print("John & Jacob");
  delay(2000);
  lcd.clear();
}

// Updates the current time variables from the RTC
void updateCurrentTime(const DateTime& now) {
  currentHour = now.hour();
  currentMinute = now.minute();
  currentSecond = now.second();
}

// Displays the current time and date on the LCD
void displayTimeAndDate(const DateTime& now) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(currentHour);
  lcd.print(":");
  lcd.print(currentMinute);
  lcd.print(":");
  lcd.print(currentSecond);
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day());
  lcd.print("/");
  lcd.print(now.month());
  lcd.print("/");
  lcd.print(now.year());
}

// Checks if the set alarm button is pressed and initiates alarm setting
void checkAndSetAlarm() {
  if (digitalRead(SET_ALARM_BUTTON_PIN) == LOW) {
    setAlarmTime();
  }
}

// Allows the user to set the alarm time
void setAlarmTime() {
  // Detailed implementation of setting alarm time goes here
}

// Checks the state of the day button and updates the current date
void checkDayButton() {
  if (digitalRead(DAY_BUTTON_PIN) == LOW) {
    currentDate = (currentDate + 1) % 7;
    delay(100); // Debounce delay to avoid multiple reads
  }
}

// Operates the servo based on the current date
void operateServo() {
  int angle = REFILL_ANGLE + currentDate * ANGLE_PER_DAY;
  servoTest.write(angle);
  delay(600); // Allows time for the servo to move to position
}

// Additional functions like beep() and match() can also have detailed comments explaining their logic
