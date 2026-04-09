// A2とA3の圧力センサ値を読む

#include <MP.h>  // MPLog用
#define INTERVAL_US 10000

#define ADC_RESOLUTION 1024
#define SENSOR_BIAS_VOLTAGE 1.0
#define MAXIMUM_VOLTAGE 5.0
#define SENSOR_MAXIMUM_VALUE 1000

int16_t pressure_sensor_value = 0;
double measured_pressure_kPa[2] = {0.0, 0.0};

unsigned int flip(){
    MPLog("A2:%.2lf,A3:%.2lf,A4:%.2lf,A5:%.2lf\r\n", measured_pressure_kPa[0], measured_pressure_kPa[1], 0, 0);
    return INTERVAL_US;
}

void setup() {
  // put your setup code here, to run once:
  attachTimerInterrupt(flip, INTERVAL_US);
}

void loop() {
  // put your main code here, to run repeatedly:
  pressure_sensor_value = analogRead(A2);
  measured_pressure_kPa[0] = ((double)pressure_sensor_value/ADC_RESOLUTION*MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)/(MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)*SENSOR_MAXIMUM_VALUE;
  pressure_sensor_value = analogRead(A3);
  measured_pressure_kPa[1] = ((double)pressure_sensor_value/ADC_RESOLUTION*MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)/(MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)*SENSOR_MAXIMUM_VALUE;
}
