#include <SPI.h>

const byte CMD_READ_ID = 0x9F;
const int  CHIP_SELECT_PIN = 10;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV128);

  pinMode(CHIP_SELECT_PIN, OUTPUT);
  digitalWrite(CHIP_SELECT_PIN, HIGH);
  delay(500);
}

void loop() {
  digitalWrite(CHIP_SELECT_PIN, LOW);
  SPI.transfer(CMD_READ_ID);
  byte data;
  Serial.print("MFID=");
  for (int i = 0; i < 3; i++) {
    data = SPI.transfer(0);
    Serial.print(data, HEX);
    Serial.print(" ");
  }
  Serial.print("\n");
  digitalWrite(CHIP_SELECT_PIN, HIGH);
  delay(1000);
}
