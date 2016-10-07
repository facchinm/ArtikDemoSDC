
#include <WiFi101.h>
#include <ArduinoCloud.h>

/////// Wifi Settings ///////
char ssid[] = "officine1";
char pass[] = "OfficineRulez";

// Arduino Cloud settings and credentials
const char userName[]   = "facchinm";
const char thingName[] = "MKR1000";
const char thingId[]   = "735ea5d1-08c9-4cb1-aed8-fbae239f8973";
const char thingPsw[]  = "5da046ee-4b4d-41c5-84eb-7f3efdcc96f5";

WiFiSSLClient sslClient;

// build a new object "MKR1000"
ArduinoCloudThing MKR1000;

void setup() {
  Serial.begin (9600);

  pinMode(0, OUTPUT);
  pinMode(A5, OUTPUT);
  digitalWrite(0, LOW);
  digitalWrite(A5, HIGH);

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // unsuccessful, retry in 4 seconds
    Serial.print("failed ... ");
    delay(4000);
    Serial.print("retrying ... ");
  }

  MKR1000.begin(thingName, userName, thingId, thingPsw, sslClient);
  MKR1000.enableDebug();
  // define the properties
  MKR1000.addProperty("analog", INT, RW);
  MKR1000.addExternalProperty("Artik2", "analog", INT);
}

void loop() {
  MKR1000.poll();

  MKR1000.writeProperty("analog", String(analogRead(A6)));
  
  delay(100);
}

