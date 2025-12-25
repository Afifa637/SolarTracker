/***************************************************
 * ESP32 Solar Tracker + Blynk IoT + INA219 + 2 Relays
 * 1-axis tracking with 2 LDRs
 ***************************************************/
#define BLYNK_TEMPLATE_ID "TMPL6euw4oevC"
#define BLYNK_TEMPLATE_NAME "Smart Solar Tracker"
#define BLYNK_AUTH_TOKEN "Wv9UjLdnCHNJMxvL0-dRTsDobdNfZAaj"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <ESP32Servo.h>

// ---------- WiFi ----------
char ssid[] = "YOUR_WIFI_SSID";
char pass[] = "YOUR_WIFI_PASSWORD";

// ---------- Pins ----------
const int PIN_LDR_LEFT  = 34;   // ADC1 only pins are safer while WiFi is on
const int PIN_LDR_RIGHT = 35;

const int PIN_SERVO = 18;
const int PIN_RELAY1 = 25;
const int PIN_RELAY2 = 26;

// ---------- Objects ----------
Adafruit_INA219 ina219;
Servo tracker;

// ---------- Tracking config ----------
int servoMin = 10;     // Degree adjusted according to mechanical unit
int servoMax = 170;    // Degree
int servoPos = 90;     // initially balanced
int stepSize = 5;      // step size
int diffThreshold = 120; // LDR noise gate
bool trackerEnabled = true;

// ---------- Timers ----------
BlynkTimer timer;

// ---------- Blynk handlers ----------
BLYNK_WRITE(V0) { // Relay-1
  int v = param.asInt();
  digitalWrite(PIN_RELAY1, v ? LOW : HIGH); // active LOW relays common
}

BLYNK_WRITE(V1) { // Relay-2
  int v = param.asInt();
  digitalWrite(PIN_RELAY2, v ? LOW : HIGH);
}

BLYNK_WRITE(V9) { // Tracker enable/disable
  trackerEnabled = param.asInt();
}

// ---------- Helper: clamp ----------
int clampInt(int x, int a, int b){ return x < a ? a : (x > b ? b : x); }

// ---------- Read and push power stats ----------
void pushPowerStats(){
  float busV = ina219.getBusVoltage_V();      // panel/load side voltage
  float current = ina219.getCurrent_mA() / 1000.0; // A
  float power = busV * current;

  Blynk.virtualWrite(V5, busV);
  Blynk.virtualWrite(V6, current);
  Blynk.virtualWrite(V7, power);
}

// ---------- LDR based tracking ----------
void trackSun(){
  if(!trackerEnabled) return;

  int l = analogRead(PIN_LDR_LEFT);
  int r = analogRead(PIN_LDR_RIGHT);

  int diff = l - r; // positive => left brighter, need to turn left
  if (abs(diff) > diffThreshold){
    if (diff > 0){
      servoPos -= stepSize; // turn towards left sensor
    } else {
      servoPos += stepSize; // turn towards right sensor
    }
    servoPos = clampInt(servoPos, servoMin, servoMax);
    tracker.write(servoPos);
    Blynk.virtualWrite(V8, servoPos);
  }
}

// ---------- Setup ----------
void setup(){
  Serial.begin(115200);

  // Pins
  pinMode(PIN_RELAY1, OUTPUT);
  pinMode(PIN_RELAY2, OUTPUT);
  digitalWrite(PIN_RELAY1, HIGH); // off for active-LOW relay modules
  digitalWrite(PIN_RELAY2, HIGH);

  // Servo
  tracker.attach(PIN_SERVO, 500, 2500); // pulse width for ESP32Servo
  tracker.write(servoPos);

  // I2C + INA219
  Wire.begin(21, 22); // SDA, SCL
  if (!ina219.begin()){
    Serial.println("INA219 not found. Check wiring.");
  }

  // WiFi + Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Timers
  timer.setInterval(500L, trackSun);       // tracking loop
  timer.setInterval(1000L, pushPowerStats);// power telemetry
}

// ---------- Loop ----------
void loop(){
  Blynk.run();
  timer.run();
}