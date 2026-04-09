// A2とA3の圧力センサ値を読む
// EVS2はランプ入力

#include <MP.h>  // MPLog用
#include "MCP4726.hpp"
#define INTERVAL_US 10000

#define ADC_RESOLUTION 1024
#define SENSOR_BIAS_VOLTAGE 1.0
#define MAXIMUM_VOLTAGE 5.0
#define SENSOR_MAXIMUM_VALUE 1000

#define EVS2_OPERATION_BIAS_VOLTAGE 0.0
#define EVS2_OPERATION_MAXIMUM_PRESSURE_KPA 500

#define PIN_REGULATOR_SENSE A0

#define OUTPUT_MAXIMUM_PRESSURE_KPA 70
#define OUTPUT_MINIMUM_COUNT_10US 600
#define OUTPUT_MAXIMUM_COUNT_10US 100
#define OUTPUT_LAMP_SPEED_KPA_10US 0.1


int16_t pressure_sensor_value = 0;
double measured_pressure_kPa[2] = {0.0, 0.0};

double evs2_operation_pressure_kPa = 0.0;
int16_t evs2_operation_value = 0.0;

uint16_t count = 0;
#define STATE_OUTPUT_MINIMUM 0
#define STATE_OUTPUT_INCREASE 1
#define STATE_OUTPUT_MAXIMUM 2
#define STATE_OUTPUT_DECREASE 3
int8_t state = STATE_OUTPUT_MINIMUM;

int operationToAnalogValue(double operation_value, int dac_resolution, double maximum_voltage, double bias_voltage, double maximum_operation){
  return (operation_value/maximum_operation*(maximum_voltage-bias_voltage) + bias_voltage)/maximum_voltage*dac_resolution;
}

unsigned int flip(){
    MPLog("A2:%.2lf,A3:%.2lf,A4:%.2lf,A5:%.2lf\r\n", measured_pressure_kPa[0], measured_pressure_kPa[1], evs2_operation_pressure_kPa, 0);
    count++;
    return INTERVAL_US;
}

void setup() {
  // put your setup code here, to run once:
  MCP4726init();
  delay(300);
  attachTimerInterrupt(flip, INTERVAL_US);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  pressure_sensor_value = analogRead(A2);
  measured_pressure_kPa[0] = ((double)pressure_sensor_value/ADC_RESOLUTION*MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)/(MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)*SENSOR_MAXIMUM_VALUE;
  pressure_sensor_value = analogRead(A3);
  measured_pressure_kPa[1] = ((double)pressure_sensor_value/ADC_RESOLUTION*MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)/(MAXIMUM_VOLTAGE-SENSOR_BIAS_VOLTAGE)*SENSOR_MAXIMUM_VALUE;
  
  if (count%10 == 0){
    MCP4726WriteVal(operationToAnalogValue(evs2_operation_pressure_kPa, MCP4726_DAC_RESOLUTION, MAXIMUM_VOLTAGE, EVS2_OPERATION_BIAS_VOLTAGE, EVS2_OPERATION_MAXIMUM_PRESSURE_KPA));
  }

  switch(state){
    case STATE_OUTPUT_MINIMUM:
      evs2_operation_pressure_kPa = 0;
      if (count > OUTPUT_MINIMUM_COUNT_10US){
        count = 0;
        state = STATE_OUTPUT_INCREASE;
      }
      break;
    case STATE_OUTPUT_INCREASE:
      evs2_operation_pressure_kPa = OUTPUT_LAMP_SPEED_KPA_10US * count;
      if (evs2_operation_pressure_kPa > OUTPUT_MAXIMUM_PRESSURE_KPA){
        count = 0;
        state = STATE_OUTPUT_MAXIMUM;
      }
      break;
    case STATE_OUTPUT_MAXIMUM:
      evs2_operation_pressure_kPa = OUTPUT_MAXIMUM_PRESSURE_KPA;
      if (count > OUTPUT_MAXIMUM_COUNT_10US){
        count = 0;
        state = STATE_OUTPUT_DECREASE;
      }
      break;
    case STATE_OUTPUT_DECREASE:
      evs2_operation_pressure_kPa = 1.0*OUTPUT_MAXIMUM_PRESSURE_KPA - OUTPUT_LAMP_SPEED_KPA_10US * count;
      if (evs2_operation_pressure_kPa < 0){
        count = 0;
        state = STATE_OUTPUT_MINIMUM;
      }
      break;

  }
}
