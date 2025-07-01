# RFID Programmer

An ESP32-based RFID card programmer using the RC522 module. This project allows you to write data to MIFARE RFID cards with multiple operation modes including automatic programming, manual mode, and card reading.

## Getting Started

### Prerequisites
- VS Code with PlatformIO extension installed ✅
- ESP32 development board
- RC522 RFID module
- MIFARE RFID cards
- Jumper wires for connections
- USB cable for programming and power

### Hardware Setup
1. Connect your ESP32 board to your computer via USB
2. Connect the RC522 RFID module to your ESP32 using the following pin connections:
   - **SDA**: GPIO 5
   - **SCK**: GPIO 18
   - **MOSI**: GPIO 23
   - **MISO**: GPIO 19
   - **RST**: GPIO 21
   - **GND**: GND
   - **3.3V**: 3.3V
3. Make sure the drivers are installed (usually automatic on Windows 10/11)

### Building and Uploading
1. Open this project in VS Code
2. PlatformIO should automatically detect the project
3. Click the PlatformIO icon in the sidebar
4. Use the following commands:
   - **Build**: Click the checkmark (✓) button or use `Ctrl+Alt+B`
   - **Upload**: Click the arrow (→) button or use `Ctrl+Alt+U`
   - **Serial Monitor**: Click the plug icon or use `Ctrl+Alt+S`

### Project Structure
```
├── src/
│   └── main.cpp          # Main application code
├── platformio.ini        # PlatformIO configuration
├── .github/
│   └── copilot-instructions.md  # Copilot customization
└── README.md            # This file
```

### Sample Program
The main program (`src/main.cpp`) provides:
- **Auto Mode**: Automatically programs consecutive numbers starting from your chosen number
- **Manual Mode**: Allows you to enter specific numbers to write to cards
- **Read Mode**: Reads and displays the data stored on RFID cards
- **Real-time mode switching**: Change modes instantly without restarting
- **Dynamic starting numbers**: Set custom starting point for auto mode
- **Optimized performance**: Fast card detection and writing

### Configuration
The `platformio.ini` file is configured for:
- **Platform**: Espressif32
- **Board**: ESP32 Dev Module
- **Framework**: Arduino
- **Monitor Speed**: 115200 baud
- **Upload Speed**: 115200 baud
- **Libraries**: MFRC522 RFID library

### Adding Libraries
The MFRC522 library is already included. To add additional libraries, modify the `lib_deps` section in `platformio.ini`:
```ini
lib_deps = 
    miguelbalboa/MFRC522@^1.4.10
    bblanchon/ArduinoJson@^6.21.2
    ottowinter/ESPAsyncWebServer-esphome@^3.0.0
```

### Usage Instructions
1. **Start the program**: Upload the code and open the Serial Monitor
2. **Choose mode**: 
   - Enter `auto` for automatic sequential numbering
   - Enter `manual` for custom number input
   - Enter `read` to read card data
3. **Auto Mode**: 
   - Enter `auto` to activate auto mode
   - When prompted, enter your desired starting number (e.g., `25`)
   - Present cards to automatically write consecutive numbers (25, 26, 27, etc.)
4. **Manual Mode**: Enter a number, then present the card to write that number
5. **Read Mode**: Present cards to display their stored data
6. **Switch modes**: Type the new mode name at any time to switch instantly

### Troubleshooting
- If upload fails, make sure the ESP32 is connected and the correct COM port is detected
- For detailed troubleshooting, see `ESP32_TROUBLESHOOTING.md`
- For hardware diagnostic steps, see `HARDWARE_DIAGNOSTIC.md`
- Check project status in `PROJECT_STATUS.md`
- Press and hold the BOOT button on the ESP32 during upload if auto-reset doesn't work
- Check that no other program is using the serial port

### Next Steps
- Modify `src/main.cpp` to implement your project
- Add necessary libraries in `platformio.ini`
- Explore ESP32 features like WiFi, Bluetooth, sensors, etc.

## Useful Resources
- [ESP32 Arduino Core Documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [PlatformIO ESP32 Platform](https://docs.platformio.org/en/latest/platforms/espressif32.html)
- [ESP32 Pinout Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)

## Features

### Enhanced Auto Mode
- **Dynamic Starting Numbers**: Choose any starting number for sequential programming
- **Flexible Range**: Start from 0 or any positive number
- **Continuous Operation**: Once started, cards are programmed sequentially without interruption
- **Real-time Feedback**: Shows current number and next number to be written

### Three Operation Modes
- **Auto Mode**: Sequential numbering with user-defined starting point
- **Manual Mode**: Individual number entry for each card
- **Read Mode**: Display stored data from existing cards

### Performance Optimizations
- **High-Speed SPI**: 10MHz SPI frequency for faster card operations
- **Maximum Antenna Gain**: Optimized for better card detection range
- **Instant Mode Switching**: Change modes without device restart
- **Non-blocking Operation**: Responsive to mode changes even during card operations
