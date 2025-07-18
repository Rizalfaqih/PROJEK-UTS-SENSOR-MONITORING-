//....................Magang UPI 2025......................//
const int potPin = A0;
const int ledPin = 9;
const int buttonPin = 2;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  int potValue = analogRead(potPin);
  int ledBrightness = map(potValue, 0, 1023, 0, 255);

  if (buttonState == HIGH) {
    analogWrite(ledPin, ledBrightness);
  } else {
    analogWrite(ledPin, 0);
  }

  // Tampilkan status tombol sebagai NYALA / MATI
  Serial.print("Tombol: ");
  if (buttonState == HIGH) {
    Serial.print("NYALA");
  } else {
    Serial.print("MATI");
  }

  Serial.print(" | Potensio: ");
  Serial.print(potValue);
  Serial.print(" | Kecerahan LED: ");
  Serial.println(ledBrightness);

  delay(100);
}
