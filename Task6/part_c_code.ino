#include <Servo.h>
#include <LiquidCrystal.h>

// --- Pin definitions ---
const int btnPin     = 2;
const int potPin     = A0;
const int servoPinA  = 9;
const int servoPinB  = 10;
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 8;

// --- Objects ---
Servo servoA, servoB;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// --- State vars ---
bool activeServo = false;  // false = A, true = B
int lastBtnState = LOW;
unsigned long lastDebounce = 0;
const unsigned long debounceDelay = 50;

void setup() {
  // initialize serial (optional)
  Serial.begin(9600);

  // attach servos
  servoA.attach(servoPinA);
  servoB.attach(servoPinB);

  // button input
  pinMode(btnPin, INPUT);

  // LCD init
  lcd.begin(16, 2);
  lcd.print("Servo Controller");
  delay(1000);
  lcd.clear();
}

void loop() {
  // --- Button debouncing & toggle ---
  int reading = digitalRead(btnPin);
  if (reading != lastBtnState) {
    lastDebounce = millis();
  }
  if ((millis() - lastDebounce) > debounceDelay) {
    static int btnState = LOW;
    if (reading != btnState) {
      btnState = reading;
      if (btnState == HIGH) {
        activeServo = !activeServo;
      }
    }
  }
  lastBtnState = reading;

  // --- Read pot & map to angle ---
  int potVal = analogRead(potPin);
  int angle = map(potVal, 0, 1023, 0, 180);

  // --- Apply to active servo ---
  if (!activeServo) {
    servoA.write(angle);
  } else {
    servoB.write(angle);
  }

  // --- Update LCD ---
  lcd.setCursor(0, 0);
  lcd.print("Active: Servo ");
  lcd.print(activeServo ? 'B' : 'A');
  lcd.print("   "); // clear trailing
  lcd.setCursor(0, 1);
  lcd.print("Angle: ");
  lcd.print(angle);
  lcd.print("   ");

  // small delay
  delay(50);
}
