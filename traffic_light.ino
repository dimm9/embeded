const int red = 11;
const int yellow = 12;
const int green = 13;
const int buttonPin = 2;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  digitalWrite(red, HIGH);
  if (digitalRead(buttonPin) == HIGH) {
    runTrafficLight();
  }
}

void runTrafficLight() {
  digitalWrite(red, HIGH);
  delay(3000);
  digitalWrite(red, LOW);

  digitalWrite(yellow, HIGH);
  delay(1000);
  digitalWrite(yellow, LOW);

  digitalWrite(green, HIGH);
  delay(3000);
  digitalWrite(green, LOW);

  digitalWrite(yellow, HIGH);
  delay(1000);
  digitalWrite(yellow, LOW);
}
