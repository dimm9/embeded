#include <LiquidCrystal.h>

// LCD pin connections: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int analogPin = A0;
float voltage = 0.0;

void setup() {
  lcd.begin(16, 2);             // Initialize LCD (16 cols, 2 rows)
  lcd.setCursor(0, 0);            // pierwszy wiersz
  lcd.print("Digital Voltmeter"); 
  delay(2000);                   
}

void loop() {
  int raw = analogRead(analogPin);       // odczyt z A0 (0-1023)
  voltage = raw * (5.0 / 1023.0);        //zamiana na napięcie 0-5V
  //voltage *= 6.0;                      // przelicz na 0-30V (dzielnik ×6)

  lcd.setCursor(0, 0);
  lcd.print("Voltage: ");
  lcd.print(voltage, 2);
  lcd.print(" V     ");

  delay(500);
}
