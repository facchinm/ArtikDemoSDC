#include <WiFi.h>
#include <SPI.h>
#include <TelegramBot.h>
#include "rgb_lcd.h"
#include "secrets.h"

String last_sender;
String last_text;

#define Serial DebugSerial

WiFiSSLClient client;
TelegramBot bot (BotToken, BotName, BotUsername, client);
rgb_lcd lcd;

void setup() {

  Serial.begin(115200);
  while (!Serial) {}
 
  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  Console.begin();

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(2000);
  }

  Serial.println("");
  Serial.println("WiFi connected");

  bot.begin();
  lcd.begin(16, 2);
  Serial.println("begin done");
}

void loop() {

  message m = bot.getUpdates(); // Read new messages
  if (m.text.length() > 0) {
    lcd.clear();
    m.text = findRGBCode(m.text);
    Console.println(m.text);
    lcd.setCursor(0, 0);
    lcd.print(m.sender + " wrote:");
    lcd.setCursor(0, 1);
    lcd.print(m.text);

    if (last_sender == m.sender && last_text == m.text) {
      return;
    }

    String response = "Thanks " + m.sender + " for joining us at SDC 2016";
    String fortune = System.runShellCommand("fortune -n 100 -s linux linuxcookie computers science startrek ");
    Serial.println(fortune);
    fortune.replace("\n", " ");
    fortune.replace("\r", " ");
    fortune.replace("\t", " ");
    bot.sendMessage(m.chat_id, response + "%0A%0AHere's your quote of the day (provided by fortune):%0A%0A" + fortune);
    last_sender = m.sender;
    last_text = m.text;
  }
}

String findRGBCode(String text) {
  int r, g, b;
  r = text.toInt();
  if (r > 0) {
    int index = text.indexOf(":");
    Console.println(index);
    g = text.substring(index + 1).toInt();
    int index2 =  text.substring(index + 1).indexOf(":");
    Console.println(index2);
    b = text.substring(index + index2 + 2).toInt();
    //Console.println(String(r) + " " + String(g) + " " + String(b));
    int index3 =  text.substring(index + index2 + 2).indexOf(":");
    text = text.substring(index + index2 + index3 + 3);
    lcd.setRGB(r, g, b);
  }
  return text;
}
