# 🌤 ESP32 Hong Kong Weather Display

A simple yet powerful **ESP32-based weather station** that fetches **Hong Kong 9-day weather forecasts** from the [Hong Kong Observatory Open Data API](https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=fnd&lang=en) and displays the next two days’ **temperature** and **humidity** on a 16×2 I²C LCD.

---

## ✨ Features

- 📡 **Wi-Fi Auto-Setup** using [WiFiManager](https://github.com/tzapu/WiFiManager) — no hardcoded credentials.
- 🔄 **Reset Wi-Fi** by holding the **BOOT** button (GPIO0) at startup.
- 🌐 **Secure HTTPS** requests to the Hong Kong Observatory API.
- 📊 Displays **min/max temperature** (°C) and **min/max relative humidity** (%) for the next two days.
- ⏱ Auto-refresh every **3 hours**.
- 🖥 Clear, easy-to-read **16×2 LCD** output.

---

## 🖼 Example Display

```
1:18-25°C 65-90%
2:19-26°C 60-85%
```
- **1:** Tomorrow’s forecast  
- **2:** Day after tomorrow’s forecast

---

## 🛠 Hardware Requirements

- **ESP32 Dev Board** (e.g., ESP32-DevKitC, NodeMCU-32S)
- **16×2 I²C LCD** (default address `0x27`, adjustable in code)
- **Push Button** (optional, for Wi-Fi reset — connected to GPIO0)
- USB cable for programming

---

## 📦 Software Requirements

Install these libraries in the Arduino IDE:

| Library | Purpose |
|---------|---------|
| [WiFiManager](https://github.com/tzapu/WiFiManager) | Wi-Fi setup & captive portal |
| WiFiClientSecure | Secure HTTPS connection |
| HTTPClient | HTTP requests |
| [ArduinoJson](https://arduinojson.org/) | Parse JSON weather data |
| [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C) | LCD control |

---

## ⚙️ Wiring

| ESP32 Pin | LCD Pin | Notes |
|-----------|--------|-------|
| 3.3V      | VCC    | Power |
| GND       | GND    | Ground |
| GPIO21    | SDA    | I²C Data |
| GPIO22    | SCL    | I²C Clock |

**Reset Button:** Connect between **GPIO0** and **GND**.

---

## 🚀 Setup & Usage

1. **Clone this repo** or copy the `.ino` file into Arduino IDE.
2. **Install required libraries** (see above).
3. **Connect your hardware** as per wiring table.
4. **Upload the code** to your ESP32.
5. On first boot, the ESP32 will start a Wi-Fi AP named:
   ```
   ESP32-Weather
   ```
6. Connect to it with your phone/PC, open the captive portal, and enter your Wi-Fi credentials.
7. The LCD will show **WiFi Connected** and then display the weather.
8. To **reset Wi-Fi credentials**, hold the BOOT button while powering on.

---

## 🔄 How It Works

1. **Wi-Fi Connection**  
   Uses WiFiManager to connect or create a setup portal.
2. **Data Fetching**  
   Every 3 hours, sends an HTTPS GET request to the Hong Kong Observatory API.
3. **JSON Parsing**  
   Extracts min/max temperature and humidity for the next two days.
4. **LCD Display**  
   Formats and displays the data in a compact, readable format.

---

## 📌 Notes

- Default LCD I²C address is `0x27`. Change in:
  ```cpp
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  ```
- API endpoint is set to English. Change `lang=en` to `lang=tc` for Traditional Chinese.
- The ESP32’s BOOT button is used for Wi-Fi reset, but you can change the pin.

---

## 📜 License

This project is open-source under the **MIT License** — feel free to modify and share.

---

## 🙌 Credits

- [Hong Kong Observatory Open Data API](https://data.weather.gov.hk/weatherAPI/)
- [WiFiManager](https://github.com/tzapu/WiFiManager)
- [ArduinoJson](https://arduinojson.org/)
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)
