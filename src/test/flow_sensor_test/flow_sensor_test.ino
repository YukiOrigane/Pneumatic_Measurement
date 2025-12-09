
#include <MP.h>  // MPLog用
#define PIN_FLOW_SENSOR A1
#define INTERVAL_US 10000

#define ADC_RESOLUTION 1024
#define SENSOR_BIAS_VOLTAGE 1.0
#define MAXIMUM_VOLTAGE 5.0
#define SENSOR_MAXIMUM_VALUE 20

int16_t flow_sensor_value = 0;
double measured_flow_l_min = 0.0;

unsigned int flip(){
    MPLog("raw value: %d, measured pressure: %.2lf L/min \r\n", flow_sensor_value, measured_flow_l_min);
    return INTERVAL_US;
}

void setup() {
  // put your setup code here, to run once:
  attachTimerInterrupt(flip, INTERVAL_US);
}

void loop() {
  // put your main code here, to run repeatedly:
  flow_sensor_value = analogRead(PIN_FLOW_SENSOR);
  measured_flow_l_min = ((double)flow_sensor_value/ADC_RESOLUTION*MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)/(MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)*SENSOR_MAXIMUM_VALUE;
}
