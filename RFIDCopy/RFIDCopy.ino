
#include <Wire.h>
#include "MFRC522_I2C.h"
#include <M5Stack.h>

// 0x28 is i2c address on SDA. Check your address with i2cscanner if not match.
MFRC522 mfrc522(0x28);   // Create MFRC522 instance.

enum state {
  read_mode,
  write_mode
} currentState;

bool readUID = false;

// UID can be several sizes.
byte UID[20];
uint8_t UIDLength = 0;

void displayReadMode() {
  M5.Lcd.println(F("RFID Cloner. V1.0"));
  M5.Lcd.setTextFont(2);
  M5.Lcd.println(F("(Press 'A' key to write after reading card...)"));
  M5.Lcd.println();
  M5.Lcd.setTextFont(4);
  M5.Lcd.println(F("Ready to READ..."));
  M5.Lcd.println();
}

void displayWriteMode() {
  M5.Lcd.println(F("RFID Cloner. V1.0"));
  M5.Lcd.setTextFont(2);
  M5.Lcd.println(F("(Press 'A' key to read a new card...)"));
  M5.Lcd.println();
  M5.Lcd.setTextFont(4);
  M5.Lcd.println(F("Ready to WRITE..."));
  M5.Lcd.println();
  displayUID();
}
void beep_attention() {
  M5.Speaker.tone(882, 40);
  delay(100);
  M5.Speaker.mute();
}

void beep_error() {
  M5.Speaker.tone(495, 60);
  delay(60);
  M5.Speaker.mute();
}

void beep_writeOK() {
  M5.Speaker.tone(661, 60);
  delay(60);
  M5.Speaker.tone(882, 100);
  delay(100);
  M5.Speaker.mute();
}

void cls() {
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
}

void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextFont(4);
  M5.Lcd.setCursor(0, 0);
  Serial.begin(115200);           // Initialize serial communications with the PC
  Wire.begin();                   // Initialize I2C
  mfrc522.PCD_Init();
  currentState = read_mode;
  displayReadMode();
}

void loop() {
  M5.update();

  if (M5.BtnA.wasReleased() && readUID) {
    cls();
    switch (currentState) {
      case read_mode:
        currentState = write_mode;
        displayWriteMode();
        break;
      case write_mode:
        currentState = read_mode;
        displayReadMode();
        readUID = false;
        break;
    }
  }

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  cls();

  switch (currentState) {
    case read_mode: displayReadMode(); readCard(); break;
    case write_mode: displayWriteMode(); writeCard(); break;
  }

  mfrc522.PICC_HaltA();
}

void readCard() {
  MFRC522::PICC_Type piccType = (MFRC522::PICC_Type)mfrc522.PICC_GetType(mfrc522.uid.sak);
  M5.Lcd.print(F(""));
  M5.Lcd.print(mfrc522.PICC_GetTypeName(piccType));
  M5.Lcd.print(F(" (SAK "));
  M5.Lcd.print(mfrc522.uid.sak);
  M5.Lcd.print(")\r\n");
  if (  piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    M5.Lcd.setTextColor(RED);
    M5.Lcd.println(F("Not a MIFARE Classic card."));
    M5.Lcd.setTextColor(YELLOW);
    beep_error();
    delay(1000);
    return;
  }

  M5.Lcd.println("");
  readUID = true;
  UIDLength = mfrc522.uid.size;
  for (byte i = 0; i < UIDLength; i++) {
    UID[i] = mfrc522.uid.uidByte[i];
  }
  Serial.println();
  displayUID();
  beep_attention();
  delay(1000);
}

void displayUID() {
  M5.Lcd.println(F("User ID: "));
  for (byte i = 0; i < UIDLength; i++) {
    M5.Lcd.print(UID[i] < 0x10 ? " 0" : " ");
    M5.Lcd.print(UID[i], HEX);
  }
}

void writeCard() {
  if ( mfrc522.MIFARE_SetUid(UID, (byte)UIDLength, true) ) {
    M5.Lcd.println();
    M5.Lcd.println(F("Wrote UID."));
    M5.Lcd.println();
    beep_writeOK();
  } else {
    M5.Lcd.setTextColor(RED);
    M5.Lcd.println();
    M5.Lcd.println(F("Write failed."));
    beep_error();
    M5.Lcd.setTextColor(YELLOW);
  }

  mfrc522.PICC_HaltA();
  delay(1000);
}
