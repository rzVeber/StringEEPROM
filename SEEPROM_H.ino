//#include <EEPROM.h>
#include "SEEPROM.h"

#define  chars 6

SEEPROM eep(4096, 16, chars);

char arr0[128];

void setup() {
  //  delay(5000);
  Serial.begin(115200);
  //  while (!Serial);
  //eep.read(0).toCharArray(arr0, 128);
  Serial.println();
  Serial.println("Begin to read :");
  //  eep.begin();
//  eep.restart();
  eep.write("Start", 0);
  eep.write("two", 1);
  eep.write("tree", 2);
  eep.write("four", 3);
  eep.write("five", 4);
  eep.write("Hi dev!?", 5);
  //  eep.submit();
  for (uint16_t i = 0; i < chars; i++) {
    Serial.print(i); Serial.print(" >> "); Serial.println(eep.read(i));
  }
  //Serial.println(eep.read(0));
  Serial.println("End of read.");
  //  eep.end();
}

void loop() {

}
