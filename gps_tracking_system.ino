#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// GPS and LCD setup
TinyGPSPlus gps;                    // GPS Object
SoftwareSerial ss(4, 3);             // GPS Module connected to Pins 4 (TX), 3 (RX)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD with I2C interface

const int emergencyButtonPin = 2;    // Emergency button connected to Pin 2
float initialLat, initialLon;        // Store initial coordinates
bool tracking = false;               // Tracking flag
bool emergencyButtonPressed = false;

// Function to calculate distance between two points (in meters)
float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
  return TinyGPSPlus::distanceBetween(lat1, lon1, lat2, lon2);
}

// Function to send coordinates to the mobile app via Serial (Bluetooth/WiFi)
void sendCoordinatesToApp(float lat, float lon) {
  Serial.print("Lat: ");
  Serial.print(lat, 6);
  Serial.print(", Lon: ");
  Serial.println(lon, 6);
  // Communication with the app (ESP8266/ESP32 communication to be added here)
}

void setup() {
  Serial.begin(115200);
  ss.begin(9600);   // GPS module baud rate
  lcd.begin();
  lcd.backlight();
  
  pinMode(emergencyButtonPin, INPUT_PULLUP); // Button with internal pullup
  lcd.clear();
  lcd.print("GPS Tracking");
  delay(2000);
}

void loop() {
  // Check if GPS data is available
  while (ss.available() > 0) {
    gps.encode(ss.read());
  }

  // Get current GPS location if valid
  if (gps.location.isValid()) {
    float currentLat = gps.location.lat();
    float currentLon = gps.location.lng();

    // Check if emergency button is pressed
    if (digitalRead(emergencyButtonPin) == LOW && !emergencyButtonPressed) {
      emergencyButtonPressed = true;
      initialLat = currentLat;
      initialLon = currentLon;

      // Display initial coordinates on LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lat:");
      lcd.print(initialLat, 6);
      lcd.setCursor(0, 1);
      lcd.print("Lon:");
      lcd.print(initialLon, 6);
      
      tracking = true;
    }

    // If tracking, check if the person has moved out of 10 km range
    if (tracking && calculateDistance(currentLat, currentLon, initialLat, initialLon) > 10000) {
      lcd.clear();
      lcd.print("Out of Range");

      // Send the new coordinates to mobile app for tracking
      sendCoordinatesToApp(currentLat, currentLon);
    }
  }

  delay(1000); // 1 second delay
}
