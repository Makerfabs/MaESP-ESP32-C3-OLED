#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

#define SSID "Makerfabs"
#define PWD "20160704"

// SSD1306
#define C3_SDA 2
#define C3_SCL 3
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *ntpServer = "120.25.108.11";
const long gmtOffset_sec = (-5) * 60 * 60; // China+8
const int daylightOffset_sec = 0;
struct tm timeinfo;
String global_time = "No time";

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
    ntp_init();
}

void loop()
{
    display.setCursor(0, 40); // Start at top-left corner
    display.setTextColor(SSD1306_BLACK); // Draw white text
    display.println(global_time);
    fresh_time();

    display.setCursor(0, 40); // Start at top-left corner
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.println(global_time);
    display.display();

    USBSerial.println(global_time);
    delay(1000);
}

void logoshow(void)
{
    display.clearDisplay();

    display.setTextSize(1);              // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    display.println(F("MaESP ESP32-C3"));
    display.setCursor(0, 20); // Start at top-left corner
    display.println(F("NTP Time DEMO"));
    display.display();
    delay(2000);
}

void ntp_init()
{
    WiFi.begin(SSID, PWD);
    int connect_count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        connect_count++;
        if (connect_count > 20)
        {
            Serial.println("Wifi time out");
            break;
        }
    }
    configTime(28800, daylightOffset_sec, ntpServer);
    while (1)
    {
        fresh_time();
        if (global_time.indexOf("N") == -1)
            break;
    }
}

void fresh_time()
{
    char time_str[40] = "";
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
    }
    else
    {
        sprintf(time_str, "%02d/%02d/%02d %02d:%02d:%02d", timeinfo.tm_year + 1900, timeinfo.tm_mon, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        Serial.println(time_str);
        global_time = (String)time_str;
    }
}