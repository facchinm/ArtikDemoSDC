#include "DHT.h"
#include "rgb_lcd.h"

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
#define Serial DebugSerial
rgb_lcd lcd;

byte degree[8] = {
    0b00011,
    0b00011,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

void setup() {
  Serial.begin(9600);
  Console.begin();
  Serial.println("Starting DHT demo");
  dht.begin();
  delay(2000);
  lcd.begin(16, 2);
  lcd.createChar(0, degree);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    //Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Check if any reads failed and exit early (to try again).
  if ((int)h == 0 || (int)t == 0) {
    //Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Hum:");
  lcd.setCursor(9, 0);
  lcd.print(h);
  lcd.setCursor(15, 0);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.setCursor(9, 1);
  lcd.print(t);
  lcd.setCursor(14, 1);
  lcd.write((unsigned char)0);
  lcd.setCursor(15, 1);
  lcd.print("C");

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C \t");
  
  Console.print("Humidity: ");
  Console.print(h);
  Console.print(" %\t");
  Console.print("Temperature: ");
  Console.print(t);
  Console.println(" *C \t");
}
