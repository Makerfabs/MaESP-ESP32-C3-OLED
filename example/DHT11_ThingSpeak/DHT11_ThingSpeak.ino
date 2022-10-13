
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define SSID "Makerfabs"
#define PWD "20160704"

// SSD1306
#define C3_SDA 2
#define C3_SCL 3
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT11
DHT dht(DHTPIN, DHTTYPE);

String dht_str = "";
float h = 0.0;
float t = 0.0;

void setup()
{
    USBSerial.begin(115200);
    USBSerial.println("MaESP C3 begin");

    // LCD
    Wire.begin(C3_SDA, C3_SCL);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3C for 128x32
        USBSerial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }

    dht.begin();

    WiFi.begin(SSID, PWD);
    int connect_count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        USBSerial.print(".");
        connect_count++;
        if (connect_count > 20)
        {
            USBSerial.println("Wifi time out");
            break;
        }
    }

    logoshow();
    delay(2000);
    while (1)
    {
        if (get_dht() != 0)
        {
            weather_request();
            delay(3000);
            break;
        }
        delay(3000);
    }
}

long runtime = 0;

void loop()
{

    display.setCursor(0, 40);            // Start at top-left corner
    display.setTextColor(SSD1306_BLACK); // Draw white text
    display.println(dht_str);
    get_dht();

    display.setCursor(0, 40);            // Start at top-left corner
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.println(dht_str);
    display.display();

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
    display.println(F("DHT11 to ThingSpeak"));
    display.display();
    delay(3000);
}

int get_dht()
{

    h = dht.readHumidity();
    t = dht.readTemperature();
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t))
    {
        USBSerial.println(F("Failed to read from DHT sensor!"));
        return 0;
    }

    dht_str = "";
    dht_str = dht_str + "TEMP:" + (int)t + " C HUMI:" + (int)h + " %";
    USBSerial.println(dht_str);

    return 1;
}

void weather_request()
{
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=KCAT79TTH52MG7EA&field1=";
    url = url + t + "&field2=" + h;
    http.begin(url);
    http.GET();
    http.end();
}