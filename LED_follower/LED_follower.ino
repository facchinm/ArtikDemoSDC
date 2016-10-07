void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(6, map(analogRead(A1), 0, 4096, 0, 100));
  delay(100);
}
