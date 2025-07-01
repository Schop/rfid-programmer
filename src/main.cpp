/*
   RFID Programmer - ESP32 RFID RC522 Multi-Mode Card Programmer
   Based on work by Ananords - https://github.com/ananords
   Adapted for ESP32 with VS Code PlatformIO

   Multi-mode RFID card programmer with auto, manual, and read capabilities.
   Uses MFRC522 library for MIFARE RFID card operations.
   
   Features:
   - Auto Mode: Sequential numbering
   - Manual Mode: Custom number input
   - Read Mode: Display card data
   - Instant mode switching
   - Optimized performance
   
   -----------------------------------------------------------------------------------------
   ESP32 Pin Connections:
   -----------------------------------------------------------------------------------------
   MFRC522 RFID Reader    ESP32        Description
   SDA                    GPIO 5       SPI signal input, I2C data line, or UART data input
   SCK                    GPIO 18      SPI clock
   MOSI                   GPIO 23      SPI data input
   MISO                   GPIO 19      SPI master-in-slave-out, I2C serial clock, or UART serial output
   IRQ                    Don't connect Interrupt pin; signals the microcontroller when an RFID tag is nearby
   GND                    GND          Ground
   RST                    GPIO 21      LOW signal to put the module in power-down mode; send a HIGH signal to reset the module
   3.3V                   3.3V         Power supply (2.5-3.3V)
   -----------------------------------------------------------------------------------------
*/

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// ESP32 Pin definitions for RC522
#define RST_PIN         21          // Reset pin
#define SS_PIN          5           // SDA (SS) pin

// Function prototypes
void autoModus();
void manualModus();
void readModus();

String mode;
int currentNumber = 7; //NUMBER TO START FROM

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

void setup() {
  Serial.begin(115200);      // Initialize serial communications with the PC (ESP32 standard)
  SPI.begin();               // Init SPI bus
  
  // Optimize SPI speed for faster RFID operations
  SPI.setFrequency(10000000); // Set SPI to 10MHz (faster than default)
  
  mfrc522.PCD_Init();        // Init MFRC522 card
  
  // Set antenna gain to maximum for better performance
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  
  Serial.println(F("\n=== ESP32 RFID Card Writer ==="));
  Serial.println(F("Write 'manual' for Manual mode, 'auto' for Automatic mode, or 'read' to read cards"));
}

void loop() {

  if (Serial.available()) {

    mode = Serial.readStringUntil('\n');
    mode.trim(); // Remove any whitespace/newlines

    if (mode == "auto" || mode == "manual" || mode == "read") {
      Serial.println(" ");
      Serial.println("Device is now in " + mode + " mode");
      
      if (mode == "read") {
        Serial.println(F("Place the card on the reader to read its number"));
      } else {
        Serial.println(F("Place the card on the reader and hold it there to write song number data to the card"));
        if (mode == "auto") Serial.println((String)"Number to be written to card: " + currentNumber);
      }
      Serial.println(F("(Type a new mode anytime to switch)"));
    }

    if (mode != "auto" && mode != "manual" && mode != "read") {
      Serial.println("Wrong value, write 'auto', 'manual', or 'read'");
    }
  }

  // Execute the current mode functions (these check for cards)
  if (mode == "auto") {
    autoModus();
  } else if (mode == "manual") {
    manualModus();
  } else if (mode == "read") {
    readModus();
  }
}

void autoModus() {
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (mfrc522.PICC_IsNewCardPresent()) {

    Serial.println(" ");

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
      return;
    }

    Serial.print(F("Card UID:"));    //Dump UID
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }

    Serial.print(F(" PICC type: "));   // Dump PICC type
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    byte buffer[16];
    MFRC522::StatusCode status;
    byte block = 1;
    byte len;

    // Convert buffer to char* for sprintf
    len = sprintf((char*)buffer, "%d", currentNumber);
    for (byte i = len; i < 16; i++) buffer[i] = ' ';  // Pad with spaces

    // Authenticate using key A
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }
    else Serial.println(F("PCD_Authenticate() success"));

    // Write block
    status = mfrc522.MIFARE_Write(block, buffer, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("MIFARE_Write() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
      Serial.println(F("Try to reset the device"));
      return;
    }
    else Serial.println(F("MIFARE_Write() success"));

    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

    currentNumber++;
    Serial.println((String)"✓ Card written successfully! Next number: " + currentNumber);
    Serial.println("Put a new card on the reader...");
  }
}


void manualModus() {
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(" ");
  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print(F(" PICC type: "));   // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // Ask for song number data
  Serial.println(F("Type any number and hit send/enter (or type new mode to switch):"));
  
  // Wait for user input without blocking mode changes
  String userInput = "";
  unsigned long startTime = millis();
  const unsigned long timeout = 30000; // 30 seconds timeout
  
  while (userInput == "" && (millis() - startTime) < timeout) {
    if (Serial.available()) {
      userInput = Serial.readStringUntil('\n');
      userInput.trim();
      break;
    }
    delay(10); // Small delay to prevent overwhelming the CPU
  }
  
  // Check if user wants to change mode
  if (userInput == "auto" || userInput == "read" || userInput == "manual") {
    mode = userInput; // Change the global mode
    Serial.println("Mode changed to: " + mode);
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return; // Exit this function
  }
  
  // Check for timeout
  if (userInput == "") {
    Serial.println("Timeout - no number entered. Card reading cancelled.");
    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return;
  }

  // Prepare buffer with user input
  byte buffer[16];
  byte block = 1;
  MFRC522::StatusCode status;
  
  // Convert user input to buffer
  for (int i = 0; i < 16; i++) buffer[i] = ' '; // Clear buffer first
  int copyLength = (userInput.length() + 1 < 16) ? userInput.length() + 1 : 16;
  userInput.toCharArray((char*)buffer, copyLength);

  // Authenticate using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("PCD_Authenticate() success"));

  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("MIFARE_Write() success"));

  Serial.println("✓ Card written successfully with: " + userInput);
  Serial.println("Put a new card on the reader to write another number");

  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

void readModus() {
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(" ");
  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  
  Serial.print(F(" PICC type: "));   // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  byte buffer[18];  // Buffer for reading (16 bytes + 2 for CRC)
  byte block = 1;   // Block to read from
  byte size = sizeof(buffer);
  MFRC522::StatusCode status;

  // Authenticate using key A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Read block
  status = mfrc522.MIFARE_Read(block, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  } else {
    Serial.print(F("Number stored on card: "));
    
    // Convert buffer to string and remove trailing spaces
    String cardData = "";
    for (byte i = 0; i < 16; i++) {
      if (buffer[i] != ' ' && buffer[i] != 0) {
        cardData += (char)buffer[i];
      }
    }
    
    if (cardData.length() > 0) {
      Serial.println(cardData);
    } else {
      Serial.println("No number found or card is empty");
    }
  }

  Serial.println("Place another card to read, or type a new mode");
  Serial.println(" ");
  
  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}
