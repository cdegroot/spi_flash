#include "hexdump.h"

/**
 *  Quick and dirty memory-to-serial dumper
 *
 *  data points to the data, count the number of bytes
 *  that are to be dumped, and offset is just for formatting
 *  purposes - it is used to print the start address for every
 *  16 byte line.
 */
void hexDump(byte *data, long offset, short count) {
  char hexBuffer[16];
  char ascBuffer[17];
  ascBuffer[16] = '\0';
  byte linePosition = 0;
  Serial.println("");
  for (short i = 0; i < count; i++) {
      if (linePosition == 0) {
          sprintf(hexBuffer, "%08X - ", (unsigned long) (offset + i));
          Serial.print(hexBuffer);
      }
      sprintf(hexBuffer, "%02X ", (unsigned char) data[i]);
      Serial.print(hexBuffer);
      if (isprint(data[i])) {
        ascBuffer[linePosition] = data[i];
      } else {
        ascBuffer[linePosition] = '.';
      }
      linePosition++;
      if (linePosition == 16) {
        linePosition = 0;
        Serial.print("  '"); Serial.print(ascBuffer); Serial.print("'");
        Serial.println();
      }
  }
  Serial.println();
}

