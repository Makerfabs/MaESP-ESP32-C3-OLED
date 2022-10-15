#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SSD1306
#define C3_SDA 2
#define C3_SCL 3
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void logoshow(void);

void setup()
{
    USBSerial.begin(115200);
    USBSerial.println("MaESP C3 begin V1.0");

    // LCD
    Wire.begin(C3_SDA, C3_SCL);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    logoshow();
}

int loop_count = 0;

void loop()
{
    USBSerial.printf("USBSerial %d\n", loop_count);
    loop_count++;
    delay(1000);
     logoshow();
    display.display();
}

void logoshow(void)
{
  display.clearDisplay();
  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(WHITE);
  display.setTextSize(1.5);
  display.setCursor(0, 0);
  display.print(F("Hello! Makers"));   //increase F( ) in onder to put the constant in RAM to avoid the ROW not enough use in the AVR platfrom. variable  didn't use this way.
  display.setCursor(0, 20);
  display.print(F("time: "));
  display.print(millis() / 1000);
  display.print(F(" s"));
  display.setCursor(0, 40);
  display.print(F("Author: "));
  display.print(F("Makerfabs"));
      display.display();
}
