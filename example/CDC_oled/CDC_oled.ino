#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SSD1306
#define C3_SDA 2
#define C3_SCL 3
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
    USBSerial.begin(115200);
    USBSerial.println("MaESP C3 begin");

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
    delay(2000);
}

void logoshow(void)
{
    display.clearDisplay();

    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    display.println(F("MaESP ESP32-C3"));
    display.setCursor(0, 20); // Start at top-left corner
    display.println(F("OLED TEST DEMO"));
    display.display();
    delay(2000);
}