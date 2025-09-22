# 🏠 Smart Curtains IoT System

An automatic curtain control system using ESP32 and light sensors for energy-efficient home automation.

## 🔧 Components
- **ESP32 WROOM DevKit** - Main controller
- **LDR Sensor** - Light detection (GPIO0)
- **MG995 Servo Motor** - Curtain movement (GPIO4)
- **Blynk App** - Remote control

## ⚡ Features
- Automatic light-based curtain control
- Smartphone remote control via Blynk
- Manual override mode
- Cooldown mechanism to prevent rapid movements

## 🚀 Quick Setup

### 1. Wiring
```
LDR Sensor: VCC→3.3V, GND→GND, Signal→GPIO0
Servo Motor: VCC→5V, GND→GND, Signal→GPIO4
```

### 2. Code Configuration
```cpp
#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_AUTH_TOKEN "YourAuthToken"
char ssid[] = "YourWiFi";
char pass[] = "YourPassword";
```

### 3. Blynk App Widgets
| Pin | Widget | Function |
|-----|--------|----------|
| V0 | Gauge | Light level display |
| V1 | Switch | Auto/Manual mode |
| V2 | Button | Manual open |
| V3 | Button | Manual close |
| V4 | LED | Curtain status |
| V5 | Button | Emergency stop |

## 📊 Operation
- **Auto Mode**: Opens curtains when light < 300, closes when light > 700
- **Manual Mode**: Control via Blynk app buttons
- **Cooldown**: 5-second delay between movements

## 📋 Libraries Required
- Blynk
- ESP32Servo
- WiFi (built-in)

---
*Energy-efficient smart home automation for enhanced comfort and convenience.*
