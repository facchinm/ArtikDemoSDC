#include "Cron.h"
#include "rgb_lcd.h"

rgb_lcd lcd;
bool previousTouchButton;
int demoNumber = 0;
Process p;

void setup() {
  DebugSerial.begin(115200);
  lcd.begin(16, 2);
  System.runShellCommand("crontab -r");
  Cron.enableAutostart("launcher");
  lcd.setRGB(150, 240, 120);
  lcd.setCursor(0, 0);
  lcd.print("Arduino on Artik");
  lcd.setCursor(0, 1);
  lcd.print("Starting Demos");
  delay(10000);
  System.runShellCommand("dhclient wlan0");
}

void loop() {
  bool touchButton = (analogRead(A0) > 4000);
  if (previousTouchButton != touchButton) {
    previousTouchButton = touchButton;
    if (touchButton == true) {
      launchNextDemo();
    }
  }
  delay(100);
}

void launchNextDemo() {
  if (demoNumber != 0) {
    p.end();
  }
  String demo_name;
  String demo_description;

  if (demoNumber % 5 == 0)
    System.runShellCommand("/root/first_slide.sh");
  else
    System.runShellCommand("/root/next_slide.sh");
  

  switch (demoNumber % 5) {
    case 0:
      demo_name = "";
      demo_description = "Arduino on Artik";
      break;
    case 1:
      demo_name = "/root/led_follow";
      demo_description = "Light follow";
      break;
    case 2:
      demo_name = "/root/dht";
      demo_description = "DHT sensor";
      break;
    case 3:
      demo_name = "/root/telegram";
      demo_description = "TelegramBot";
      break;
    case 4:
      demo_name = "/root/m2m";
      demo_description = "M2M IoT";
      break;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Demo");
  lcd.setCursor(6, 0);
  lcd.print(String((demoNumber % 5) + 1));
  lcd.setCursor(0, 1);
  lcd.print(demo_description);
  p.begin(demo_name);
  p.runAsynchronously();

  DebugSerial.println("Demo started");

  delay(1000);

  demoNumber++;
}
