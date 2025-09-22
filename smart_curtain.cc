#define BLYNK_TEMPLATE_ID "TMPL3WRz-y3mC"
#define BLYNK_DEVICE_NAME "Smart Curtains"
#define BLYNK_AUTH_TOKEN "AUK27diH-1B_ZQgY9NplePdEgXK-0-kR"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

char ssid[] = "tplinnk";
char pass[] = "1963";

char auth[] = AUK27diH-1B_ZQgY9NplePdEgXK-0-kR;

#define LDR_PIN 0        
#define SERVO_PIN 4      

Servo continuousServo;

const int LIGHT_THRESHOLD_HIGH = 700;  
const int LIGHT_THRESHOLD_LOW = 300;   

unsigned long lastMovementTime = 0;
const unsigned long COOLDOWN_PERIOD = 5000;  

bool autoMode = true;           
bool curtainOpen = true;        
int currentLightLevel = 0;
bool manualOverride = false;

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  
  continuousServo.attach(SERVO_PIN);
  continuousServo.write(90); 
  
  pinMode(LDR_PIN, INPUT);
  
  Serial.println("Connecting to WiFi...");
  Blynk.begin(auth, ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  timer.setInterval(1000L, readSensorsAndControl);
  
  Serial.println("Smart Curtains System Initialized");
  Serial.println("==============================");
}

void loop() {
  Blynk.run();
  timer.run();
}

void readSensorsAndControl() {
  currentLightLevel = analogRead(LDR_PIN);
  
  Blynk.virtualWrite(V0, currentLightLevel);
  
  Serial.print("Light Level: ");
  Serial.print(currentLightLevel);
  Serial.print(" | Mode: ");
  Serial.print(autoMode ? "AUTO" : "MANUAL");
  Serial.print(" | Curtain: ");
  Serial.println(curtainOpen ? "OPEN" : "CLOSED");
  
  if (autoMode && !manualOverride) {
    if (millis() - lastMovementTime < COOLDOWN_PERIOD) {
      return; 
    }
    
    if (currentLightLevel > LIGHT_THRESHOLD_HIGH && curtainOpen) {
      closeCurtain();
      curtainOpen = false;
      lastMovementTime = millis();
      Serial.println("AUTO: Closing curtains - too bright");
    }
    else if (currentLightLevel < LIGHT_THRESHOLD_LOW && !curtainOpen) {
      openCurtain();
      curtainOpen = true;
      lastMovementTime = millis();
      Serial.println("AUTO: Opening curtains - too dark");
    }
  }
  
  Blynk.virtualWrite(V4, curtainOpen ? 1 : 0);
}

void openCurtain() {
  continuousServo.write(180);
  Serial.println("Opening Curtain");
  delay(2000); 
  stopCurtain();
}

void closeCurtain() {
  continuousServo.write(0);
  Serial.println("Closing Curtain");// R
  delay(2000); 
  stopCurtain();
}

void stopCurtain() {
  continuousServo.write(90);
  Serial.println("Stopping Curtain");
}

BLYNK_WRITE(V1) {
  autoMode = param.asInt(); 
  if (autoMode) {
    Serial.println("Switched to Automatic Mode");
    manualOverride = false;
  } else {
    Serial.println("Switched to Manual Mode");
    stopCurtain(); 
    manualOverride = false;
  }
}

BLYNK_WRITE(V2) {
  if (!autoMode && param.asInt() == 1) { 
    openCurtain();
    curtainOpen = true;
    manualOverride = true;
    Serial.println("MANUAL: Opening curtain");
  } else if (param.asInt() == 0) { 
    stopCurtain();
    Serial.println("MANUAL: Stopped curtain movement");
  }
}

BLYNK_WRITE(V3) {
  if (!autoMode && param.asInt() == 1) { 
    closeCurtain();
    curtainOpen = false;
    manualOverride = true;
    Serial.println("MANUAL: Closing curtain");
  } else if (param.asInt() == 0) { 
    stopCurtain();
    Serial.println("MANUAL: Stopped curtain movement");
  }
}

BLYNK_WRITE(V5) {
  if (param.asInt() == 1) {
    stopCurtain();
    manualOverride = true;
    Serial.println("EMERGENCY STOP activated");
  }
}

BLYNK_CONNECTED() {
  Serial.println("Blynk Connected!");
  Blynk.virtualWrite(V0, currentLightLevel);
  Blynk.virtualWrite(V1, autoMode ? 1 : 0);
  Blynk.virtualWrite(V4, curtainOpen ? 1 : 0);
}

void printSystemInfo() {
  Serial.println("\n=== System Status ===");
  Serial.print("WiFi Status: ");
  Serial.println(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
  Serial.print("Blynk Status: ");
  Serial.println(Blynk.connected() ? "Connected" : "Disconnected");
  Serial.print("Current Light Level: ");
  Serial.println(currentLightLevel);
  Serial.print("Auto Mode: ");
  Serial.println(autoMode ? "ON" : "OFF");
  Serial.print("Curtain Position: ");
  Serial.println(curtainOpen ? "OPEN" : "CLOSED");
  Serial.print("Manual Override: ");
  Serial.println(manualOverride ? "ACTIVE" : "INACTIVE");
  Serial.println("====================\n");
}