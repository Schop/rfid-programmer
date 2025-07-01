<!-- Use this file to provide workspace-specific custom instructions to Copilot. For more details, visit https://code.visualstudio.com/docs/copilot/copilot-customization#_use-a-githubcopilotinstructionsmd-file -->

# RFID Programmer - ESP32 Development Instructions

This is an ESP32-based RFID card programmer using PlatformIO and the Arduino framework with the RC522 RFID module.

## Development Guidelines:
- Use Arduino framework syntax and libraries
- Remember that ESP32 has dual-core architecture (PRO_CPU and APP_CPU)
- GPIO pins available: 0-39 (some have restrictions)
- Built-in LED is typically on GPIO 2
- Serial communication defaults to 115200 baud rate
- Use appropriate delay() functions to avoid watchdog timer issues
- Consider power management for battery-powered projects
- Use proper pin configurations for input/output operations

## Common ESP32 Libraries:
- MFRC522.h for RFID card operations (primary library for this project)
- WiFi.h for WiFi connectivity
- BluetoothSerial.h for Bluetooth communication
- SPIFFS.h for file system operations
- Wire.h for I2C communication
- SPI.h for SPI communication (used by RC522)
- AsyncWebServer.h for web server functionality

## Project-Specific Hardware:
- RC522 RFID module connected via SPI
- SDA: GPIO 5, SCK: GPIO 18, MOSI: GPIO 23, MISO: GPIO 19, RST: GPIO 21
- MIFARE RFID cards for data storage
- Three operation modes: auto, manual, and read

## Hardware Considerations:
- ESP32-DevKit boards typically use GPIO 2 for built-in LED
- GPIO 0 is used for bootmode selection
- Some GPIOs are input-only (34-39)
- GPIO 6-11 are connected to internal flash (avoid using)
