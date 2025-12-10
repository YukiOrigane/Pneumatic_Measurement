
/***************************************************
キー入力と制御圧力
wを送ると10kPa上昇，sを送ると10kPa減少，qで0kPaに復帰
aでランプ入力を開始．qで解除
***************************************************/

#include <MP.h>  // MPLog用
#include "MCP4726.hpp"
#define INTERVAL_US 10000

#define ADC_RESOLUTION 1024
#define SENSOR_BIAS_VOLTAGE 1.0
#define MAXIMUM_VOLTAGE 5.0
#define SENSOR_MAXIMUM_VALUE 500

#define EVS2_OPERATION_BIAS_VOLTAGE 0.0
#define EVS2_OPERATION_MAXIMUM_PRESSURE_KPA 500

#define PIN_REGULATOR_SENSE A0

int16_t evs2_pressure_feedback_value = 0;
double evs2_measured_pressure_kPa = 0.0;

double evs2_operation_pressure_kPa = 0.0;
int16_t evs2_operation_value = 0.0;

const double lamp_speed = 0.2;
const double lamp_max_pressure_kPa = 100;
bool enable_lamp_input = false;
bool is_lamp_rising = false;

double analogValueToMeasurement(int analog_sensor_value, int adc_resolution, double maximum_voltage, double sensor_bias_voltage, double maximum_measurement){
  return ((double)analog_sensor_value/adc_resolution*maximum_voltage-sensor_bias_voltage)/(maximum_voltage-sensor_bias_voltage)*maximum_measurement;
}

int operationToAnalogValue(double operation_value, int dac_resolution, double maximum_voltage, double bias_voltage, double maximum_operation){
  return (operation_value/maximum_operation*(maximum_voltage-bias_voltage) + bias_voltage)/maximum_voltage*dac_resolution;
}

unsigned int flip(){
    //MPLog("target pressure: %.2lf kPa, measured pressure: %.2lf kPa \r\n", evs2_operation_pressure_kPa, evs2_measured_pressure_kPa);
    MPLog("%.2lf, %.2lf \r\n", evs2_operation_pressure_kPa, evs2_measured_pressure_kPa);
    if (enable_lamp_input){
      if (is_lamp_rising){
        evs2_operation_pressure_kPa += lamp_speed;
        if (evs2_operation_pressure_kPa>lamp_max_pressure_kPa){
          is_lamp_rising = false;
        }
      }else{
        evs2_operation_pressure_kPa -= lamp_speed;
        if (evs2_operation_pressure_kPa<0){
          is_lamp_rising = true;
        }
      }
    }
    return INTERVAL_US;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  MCP4726init();
  delay(300);
  attachTimerInterrupt(flip, INTERVAL_US);
}

void loop() {
  // put your main code here, to run repeatedly:
  evs2_pressure_feedback_value = analogRead(PIN_REGULATOR_SENSE);
  evs2_measured_pressure_kPa = analogValueToMeasurement(evs2_pressure_feedback_value, ADC_RESOLUTION, MAXIMUM_VOLTAGE, SENSOR_BIAS_VOLTAGE, SENSOR_MAXIMUM_VALUE);
  if (Serial.available() > 0){
    int receiving_byte = Serial.read();
    if (receiving_byte == 'w'){
      evs2_operation_pressure_kPa+=10.0;
      if (evs2_operation_pressure_kPa>200.0){
        evs2_operation_pressure_kPa = 200.0;
      }
    }
    if (receiving_byte == 's'){
      evs2_operation_pressure_kPa-=10.0;
      if (evs2_operation_pressure_kPa<0.0){
        evs2_operation_pressure_kPa = 0.0;
      }
    }
    if (receiving_byte == 'q'){
      enable_lamp_input = false;
      is_lamp_rising = false;
      evs2_operation_pressure_kPa = 0.0;
    }
    if (receiving_byte == 'a'){
      enable_lamp_input = true;
      is_lamp_rising = true;
      evs2_operation_pressure_kPa = 0.0;
    }
    MCP4726WriteVal(operationToAnalogValue(evs2_operation_pressure_kPa, MCP4726_DAC_RESOLUTION, MAXIMUM_VOLTAGE, EVS2_OPERATION_BIAS_VOLTAGE, EVS2_OPERATION_MAXIMUM_PRESSURE_KPA));
  }
  if (enable_lamp_input) {
    static int cnt = 0;
    cnt++;
    if (cnt > 10){  // 送信頻度を制限
      MCP4726WriteVal(operationToAnalogValue(evs2_operation_pressure_kPa, MCP4726_DAC_RESOLUTION, MAXIMUM_VOLTAGE, EVS2_OPERATION_BIAS_VOLTAGE, EVS2_OPERATION_MAXIMUM_PRESSURE_KPA));
      cnt = 0;
    }
  }
}

