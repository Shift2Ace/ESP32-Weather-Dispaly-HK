#include <WiFiManager.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

// ==== CONFIGURE THESE ====
const char* apiUrl   = "https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=fnd&lang=en";
// =========================

// Pin for reset button
#define RESET_BUTTON_PIN  0   // GPIO0 (BOOT button on many ESP32 dev boards)

LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust I2C address if needed

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP); // Button to GND

  // Check if reset button is pressed at boot
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    lcd.clear();
    lcd.print("Resetting WiFi...");
    Serial.println("Resetting WiFi credentials...");
    WiFiManager wm;
    wm.resetSettings(); // Clear saved WiFi
    delay(2000);
    ESP.restart();
  }

  lcd.setCursor(0, 0);
  lcd.print("WiFi Setup...");

  WiFiManager wm;
  if (!wm.autoConnect("ESP32-Weather")) {
    Serial.println("Failed to connect");
    lcd.clear();
    lcd.print("WiFi Failed");
    delay(3000);
    ESP.restart();
  }

  Serial.println("WiFi connected");
  lcd.clear();
  lcd.print("WiFi Connected");
  delay(1000);

  getWeather();
}

void loop() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 10800000) { // every 3 hours
    getWeather();
    lastUpdate = millis();
  }
}

void getWeather() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    http.begin(client, apiUrl);
    int httpCode = http.GET();

    if (httpCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      DynamicJsonDocument doc(8192);
      if (deserializeJson(doc, payload)) {
        Serial.println("JSON parse error");
        return;
      }

      JsonArray forecast = doc["weatherForecast"];

      // First day
      int day1MaxT = forecast[0]["forecastMaxtemp"]["value"];
      int day1MinT = forecast[0]["forecastMintemp"]["value"];
      int day1MaxRH = forecast[0]["forecastMaxrh"]["value"];
      int day1MinRH = forecast[0]["forecastMinrh"]["value"];

      // Second day
      int day2MaxT = forecast[1]["forecastMaxtemp"]["value"];
      int day2MinT = forecast[1]["forecastMintemp"]["value"];
      int day2MaxRH = forecast[1]["forecastMaxrh"]["value"];
      int day2MinRH = forecast[1]["forecastMinrh"]["value"];

      // Format to two digits
      char minT1[3], maxT1[3], minRH1[3], maxRH1[3];
      char minT2[3], maxT2[3], minRH2[3], maxRH2[3];
      sprintf(minT1, "%02d", day1MinT);
      sprintf(maxT1, "%02d", day1MaxT);
      sprintf(minRH1, "%02d", day1MinRH);
      sprintf(maxRH1, "%02d", day1MaxRH);
      sprintf(minT2, "%02d", day2MinT);
      sprintf(maxT2, "%02d", day2MaxT);
      sprintf(minRH2, "%02d", day2MinRH);
      sprintf(maxRH2, "%02d", day2MaxRH);

      // Display
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("1:");
      lcd.print(minT1); lcd.print("-"); lcd.print(maxT1); lcd.print("\xDF"); lcd.print("C ");
      lcd.print(minRH1); lcd.print("-"); lcd.print(maxRH1); lcd.print("%");

      lcd.setCursor(0, 1);
      lcd.print("2:");
      lcd.print(minT2); lcd.print("-"); lcd.print(maxT2); lcd.print("\xDF"); lcd.print("C ");
      lcd.print(minRH2); lcd.print("-"); lcd.print(maxRH2); lcd.print("%");

    } else {
      Serial.println("HTTP Error: " + String(httpCode));
      lcd.clear();
      lcd.print("Weather Error");
    }
    http.end();
  }
}
