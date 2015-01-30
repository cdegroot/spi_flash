#include <SPI.h>
#include "hexdump.h"

//#define DEBUG

const byte CMD_READ = 0x03;
const byte CMD_READ_ID = 0x9F;
const int  CHIP_SELECT_PIN = 10;

#define SPI_DO(x) \
  digitalWrite(CHIP_SELECT_PIN, LOW); \
  x; \
  digitalWrite(CHIP_SELECT_PIN, HIGH);


#define SPI_CMD(c, x) SPI_DO(SPI.transfer(c); x)


void setup() {
  Serial.begin(57600);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16);

  pinMode(CHIP_SELECT_PIN, OUTPUT);
  digitalWrite(CHIP_SELECT_PIN, HIGH);
  delay(500);
}


/**
 *  Read manufacturer ID and print on Serial
 */
void read_mfid() {
  SPI_CMD(CMD_READ_ID,
    byte data;
    Serial.print("MFID=");
    for (int i = 0; i < 3; i++) {
      data = SPI.transfer(0);
      Serial.print(data, HEX);
      Serial.print(" ");
    }
    Serial.print("\n");
  )
}

// We buffer because I'm note sure that Serial can
// keep up with SPI bus speed.
#define BUF_SIZE 1024
byte buffer[BUF_SIZE];

#ifdef DEBUG
char textBuffer[96];
#endif

/**
 *  Read indicated block of memory and print on SERIAL.
 *  "count" is maxed to BUF_SIZE.
 */
void read_block(long address, short count) {
  if (count > BUF_SIZE) {
    count = BUF_SIZE;
  }
  byte addressBytes[3];
  addressBytes[0] = (address >> 16) & 0xff;
  addressBytes[1] = (address >> 8)  & 0xff;
  addressBytes[2] = (address)       & 0xff;
  #ifdef DEBUG
  sprintf(textBuffer, "%lX as %02X, %02X, %02X", address, addressBytes[0], addressBytes[1], addressBytes[2]);
  Serial.print(textBuffer);
  #endif
  SPI_CMD(CMD_READ,
    for (int i = 0; i < 3; i++) {
        SPI.transfer(addressBytes[i]);
    }
    for (int i = 0; i < count; i++) {
        buffer[i] = SPI.transfer(0);
    }
  );
  hexDump(buffer, address, count);
}

void dump_memory() {
  for (long i = 0; i < 1024; i++) {
    read_block(i * BUF_SIZE, BUF_SIZE);
  }
}

#define LOOP_FUNCTION dump_memory()

void loop() {
  LOOP_FUNCTION;
  delay(1000);
}


