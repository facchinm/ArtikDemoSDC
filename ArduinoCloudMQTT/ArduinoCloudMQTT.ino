
#include <WiFi.h>
#include <ArduinoCloud.h>
#include <SoftwareServo.h>

/////// Wifi Settings ///////
char ssid[] = "officine1";
char pass[] = "OfficineRulez";

// Arduino Cloud settings and credentials
const char userName[]   = "facchinm";
const char thingName[] = "Artik2";
const char thingId[]   = "3a92f702-1f1b-49b1-a137-cf3573bb1fce";
const char thingPsw[]  = "6c7c7149-e968-4a01-864e-a3200d0ba4e0";

WiFiSSLClient sslClient;

// build a new object "MKR1000"
ArduinoCloudThing MKR1000;
volatile int reading;
volatile int last_reading;

int servoPin = 7; // pin to blink, use the built-in LED
SoftwareServo myservo;  // create servo object to control a servo 

#define Serial DebugSerial

void setup() {
  Serial.begin (9600);

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);

  myservo.attach(servoPin);

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // unsuccessful, retry in 4 seconds
    Serial.print("failed ... ");
    Serial.print("retrying ... ");
  }

  MKR1000.begin(thingName, userName, thingId, thingPsw, sslClient);
  //MKR1000.enableDebug();
  // define the properties
  MKR1000.addExternalProperty("MKR1000", "analog", RW);
  
}

void loop() {
  MKR1000.poll();

  reading = MKR1000.readProperty("MKR1000", "analog").toInt();

  Serial.println(reading);
  //int reading = analogRead(A0);
  int mapped = map(reading, 15, 1024, 0, 180);
  if (mapped != last_reading) {
    myservo.write(mapped);
    last_reading = mapped;
  }

  delay(10);
}
