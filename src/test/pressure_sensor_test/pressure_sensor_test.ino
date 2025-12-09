
#include <MP.h>  // MPLog用
#define PIN_PRESSURE_SENSOR A2
#define INTERVAL_US 10000

#define ADC_RESOLUTION 1024
#define SENSOR_BIAS_VOLTAGE 1.0
#define MAXIMUM_VOLTAGE 5.0
#define SENSOR_MAXIMUM_VALUE 1000

int16_t pressure_sensor_value = 0;
double measured_pressure_kPa = 0.0;

unsigned int flip(){
    MPLog("raw value: %d, measured pressure: %.2lf kPa \r\n", pressure_sensor_value, measured_pressure_kPa);
    return INTERVAL_US;
}

void setup() {
  // put your setup code here, to run once:
  attachTimerInterrupt(flip, INTERVAL_US);
}

void loop() {
  // put your main code here, to run repeatedly:
  pressure_sensor_value = analogRead(PIN_PRESSURE_SENSOR);
  measured_pressure_kPa = ((double)pressure_sensor_value/ADC_RESOLUTION*MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)/(MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)*SENSOR_MAXIMUM_VALUE;
}
