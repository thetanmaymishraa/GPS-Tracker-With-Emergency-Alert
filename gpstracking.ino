#include <TinyGPS++.h>            // Library for GPS handling
#include <SoftwareSerial.h>       // Library for software serial communication
#include <Wire.h>                 // Library for I2C communication (for LCD)
#include <LiquidCrystal_I2C.h>    // Library for LCD

// GPS and LCD setup
TinyGPSPlus gps;                    // GPS Object for handling GPS data
SoftwareSerial ss(4, 3);             // GPS Module connected to Pins 4 (TX), 3 (RX) (GPS Rx/Tx lines)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD with I2C interface

// Emergency button setup
const int emergencyButtonPin = 2;    // Emergency button connected to Pin 2
bool emergencyButtonPressed = false; // Flag to check if the button was pressed

// Variables to store coordinates
float initialLat, initialLon;        // Store initial coordinates
bool tracking = false;               // Flag to check if tracking has started

// Function to calculate the distance between two points (in meters)
float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
  return TinyGPSPlus::distanceBetween(lat1, lon1, lat2, lon2); // Calculate distance between two GPS coordinates
}

// Function to send coordinates to the mobile app via Serial (for WiFi/Bluetooth communication)
void sendCoordinatesToApp(float lat, float lon) {
  Serial.print("Lat: ");
  Serial.print(lat, 6);
  Serial.print(", Lon: ");
  Serial.println(lon, 6);
  
  // Add here the communication code with the app (e.g., send via ESP8266/ESP32 to a server)
}

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);      // Serial monitor for debugging
  ss.begin(9600);            // GPS module baud rate for communication
  
  // Initialize LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("GPS Tracking");
  delay(2000);               // Display welcome message for 2 seconds
  
  // Emergency button setup
  pinMode(emergencyButtonPin, INPUT_PULLUP);  // Set button pin as input with internal pullup
  lcd.clear();
  lcd.print("Waiting for GPS...");
}

// Function to check if GPS is linked to the satellite
bool isGPSConnected() {
  return gps.satellites.isValid() && gps.location.isValid(); // Check if GPS data is valid (connected to satellites)
}

void loop() {
  // Check if GPS data is available
  while (ss.available() > 0) {
    gps.encode(ss.read());  // Read the GPS data from the module and process it
  }

  // Check if GPS is connected to satellites
  if (isGPSConnected()) {
    float currentLat = gps.location.lat();  // Get current latitude
    float currentLon = gps.location.lng();  // Get current longitude

    // Check if emergency button is pressed
    if (digitalRead(emergencyButtonPin) == LOW && !emergencyButtonPressed) {
      emergencyButtonPressed = true;  // Set the emergency button pressed flag
      initialLat = currentLat;        // Save the initial latitude
      initialLon = currentLon;        // Save the initial longitude

      // Display initial coordinates on LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lat:");
      lcd.print(initialLat, 6);
      lcd.setCursor(0, 1);
      lcd.print("Lon:");
      lcd.print(initialLon, 6);
      
      tracking = true;  // Start tracking when the button is pressed
    }

    // If tracking, check if the person moves out of a 10 km radius from the initial point
    if (tracking && calculateDistance(currentLat, currentLon, initialLat, initialLon) > 10000) {
      lcd.clear();
      lcd.print("Out of Range");
      
      // Send the new coordinates to the mobile app for tracking
      sendCoordinatesToApp(currentLat, currentLon);
    }
  } else {
    lcd.clear();
    lcd.print("Waiting for GPS...");
  }

  delay(1000);  // Wait for 1 second before the next iteration
}
