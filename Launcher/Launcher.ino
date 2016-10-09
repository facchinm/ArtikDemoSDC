#include "Cron.h"
#include "rgb_lcd.h"

rgb_lcd lcd;
bool previousTouchButton;
int demoNumber = 0;
Process p;

void setup() {
  DebugSerial.begin(115200);
  lcd.begin(16, 2);
  char dirNameBuffer[128];
  readlink("/proc/self/exe", dirNameBuffer, 127);
  if (strstr(dirNameBuffer, "launcher") == NULL) {
    System.runShellCommand("killall launcher");
    System.runShellCommand("crontab -r");
    Cron.enableAutostart("launcher");
  }
  lcd.setRGB(150, 240, 120);
  lcd.setCursor(0, 0);
  lcd.print("Arduino on Artik");
  lcd.setCursor(0, 1);
  System.runShellCommand("killall Xorg");
  delay(2000);
  System.runShellCommand("startx &");
  System.runShellCommand("DISPLAY=:0 xpdf Artik*pdf -fullscreen -rotateCW &");
  delay(1000);
  System.runShellCommand("dhclient wlan0");
  lcd.print("Starting Demos");
}

void loop() {
  bool touchButton = (analogRead(A0) > 4000);
  if (previousTouchButton != touchButton) {
    previousTouchButton = touchButton;
    if (touchButton == true) {
      System.runShellCommand("dhclient wlan0");
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

  if (demoNumber % 5 == 0) {
    System.runShellCommand("DISPLAY=:0 xdotool click 1");
    System.runShellCommand("DISPLAY=:0 xdotool key ctrl+Home");
  }
  else {
    System.runShellCommand("DISPLAY=:0 xdotool click 5");
  }


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

/*
  Process ping;
  ping.begin("ping -c1 www.google.it");
  if (demoNumber % 5 >= 3) {
    if (ping.run() != 0) {
      DebugSerial.println("Restarting wlan0");
      System.runShellCommand("ifconfig wlan0 down");
      System.runShellCommand("ifconfig wlan0 up");
      System.runShellCommand("killall dhclient");
      System.runShellCommand("busybox udhcpc -i wlan0");
      System.runShellCommand("avahi-daemon -r");
    }
  }
*/

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
