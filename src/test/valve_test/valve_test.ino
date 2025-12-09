#include <MP.h>  // MPLog用

#define PIN_VALVE_1 PIN_D02
#define PIN_VALVE_2 PIN_D03
#define PIN_VALVE_3 PIN_D04
#define PIN_VALVE_4 PIN_D05

#define INTERVAL_TIME_MS 1000
#define ACTIVE_TIME_MS 500

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_VALVE_1, OUTPUT);
  pinMode(PIN_VALVE_2, OUTPUT);
  pinMode(PIN_VALVE_3, OUTPUT);
  pinMode(PIN_VALVE_4, OUTPUT);
  digitalWrite(PIN_VALVE_1, 0);
  digitalWrite(PIN_VALVE_2, 0);
  digitalWrite(PIN_VALVE_3, 0);
  digitalWrite(PIN_VALVE_4, 0);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(PIN_VALVE_1, 1);
  delay(ACTIVE_TIME_MS);
  digitalWrite(PIN_VALVE_1, 0);
  delay(INTERVAL_TIME_MS);

  digitalWrite(PIN_VALVE_2, 1);
  delay(ACTIVE_TIME_MS);
  digitalWrite(PIN_VALVE_2, 0);
  delay(INTERVAL_TIME_MS);

  digitalWrite(PIN_VALVE_3, 1);
  delay(ACTIVE_TIME_MS);
  digitalWrite(PIN_VALVE_3, 0);
  delay(INTERVAL_TIME_MS);

  digitalWrite(PIN_VALVE_4, 1);
  delay(ACTIVE_TIME_MS);
  digitalWrite(PIN_VALVE_4, 0);
  delay(INTERVAL_TIME_MS);

}
