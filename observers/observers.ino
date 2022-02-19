#include <bluemicro_hid.h>
#include "actor.h"

Actor test1;
Actor test2;

void setup() {
  Serial.begin(115200);
  delay(10000);
  Serial.println("Starting up");
  test1.registerObserver(test2);
  test1.notify();
}

void loop() {
  // put your main code here, to run repeatedly:

}
