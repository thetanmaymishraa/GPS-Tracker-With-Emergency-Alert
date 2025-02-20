# GPS Tracking System with Emergency Alert

## Overview
This project is an **Arduino-based GPS tracking system** with an **emergency alert button** and an **LCD display**. It tracks the location using a GPS module and notifies when the user moves out of a **10 km range** from the initial location.

## Features
- **Real-time GPS tracking** using the TinyGPS++ library.
- **Emergency Button** to set the initial location.
- **LCD Display (16x2, I2C)** to show latitude and longitude.
- **Out-of-range detection** (10 km limit).
- **Serial communication** (Bluetooth/WiFi integration possible for mobile app tracking).

## Components Required
```markdown
- Arduino Uno (or compatible board)
- GPS Module (Neo-6M or similar)
- 16x2 LCD with I2C module
- Push Button (Emergency Alert)
- Jumper Wires
```

## Circuit Connections
```markdown
| Component | Arduino Pin |
|-----------|------------|
| GPS TX    | 4          |
| GPS RX    | 3          |
| LCD SDA   | A4         |
| LCD SCL   | A5         |
| Button    | 2          |
```

## Installation & Setup
```cpp
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
```
1. **Upload the Code** to the Arduino board.
2. **Connect the Components** as per the circuit diagram.
3. **Open Serial Monitor** (115200 baud rate) to see real-time GPS coordinates.

## Code Explanation
```markdown
- Reads GPS data from the module.
- Stores initial coordinates when the emergency button is pressed.
- Displays GPS location on LCD.
- Checks if the user moves beyond 10 km and sends an alert.
```

## Future Enhancements
```markdown
- Integrate Bluetooth/WiFi for mobile app alerts.
- Add SMS/Email alerts for emergency tracking.
- Use a battery-powered system for portability.
```

## License
```markdown
This project is open-source under the MIT License.
```

## Contribution
```markdown
Feel free to fork this repository, make improvements, and create pull requests!
```

## Author
```markdown
Developed by Tanmay Mishra
