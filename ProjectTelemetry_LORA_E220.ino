//....................Magang UPI 2025......................//
const int potPin = A0;      // Potensiometer ke A0
const int ledPin = 9;       // LED ke D9 (PWM pin)
const int buttonPin = 2;    // Push Button ke D2

void setup() {
  pinMode(ledPin, OUTPUT);          // LED sebagai output
  pinMode(buttonPin, INPUT);        // Tombol sebagai input
  Serial.begin(9600);               // Untuk debugging di serial monitor
}

void loop() {
  int buttonState = digitalRead(buttonPin);   // Baca tombol
  int potValue = analogRead(potPin);          // Baca potensiometer
  int ledBrightness = map(potValue, 0, 1023, 0, 255);  // Mapping ke 0–255

  if (buttonState == HIGH) {
    analogWrite(ledPin, ledBrightness);  // LED nyala sesuai potensiometer
  } else {
    analogWrite(ledPin, 0);              // LED mati jika tombol dilepas
  }

  // Tampilkan data di Serial Monitor
  Serial.print("Tombol: ");
  Serial.print(buttonState);
  Serial.print(" | Potensio: ");
  Serial.print(potValue);
  Serial.print(" | Kecerahan LED: ");
  Serial.println(ledBrightness);

  delay(100); // jeda 100 ms
}
