#include <MP.h>  // MPLog用
#include "MCP4726.hpp"
    #define INTERVAL_US 10000
    // double measured_pressure[NUMBER_OF_SENSOR];
    unsigned long int t_ms = 0;
    
    #define PIN_REGULATOR_SENSE A0
    #define PIN_FLOW_SENSOR A1
    #define PIN_PRESSURE_SENSOR_1 A2
    #define PIN_PRESSURE_SENSOR_2 A3
    #define PIN_PRESSURE_SENSOR_3 A4
    #define PIN_VALVE_1 PIN_D02
    #define PIN_VALVE_2 PIN_D03
    #define PIN_VALVE_3 PIN_D04
    #define PIN_VALVE_4 PIN_D05

    #define SEND_START_CODE 0x21
    #define SEND_NEXT_CODE 0x22

    const int NUMBER_OF_PRESSURE_SENSOR = 3;
    int16_t regulator_sense_value = 0;
    int16_t flow_sensor_value = 0;
    int16_t pressure_sensor_value[NUMBER_OF_PRESSURE_SENSOR];
    uint8_t target_valve_state = 0b00000000;
    int16_t target_regulator_pressure = 0;

    int16_t packet2Data(uint8_t,uint8_t);
    void data2Packet(uint8_t*, uint8_t*, int16_t);
    void dataSendEach(int16_t);
    void sendPacket();

    unsigned int flip(){
        //unsigned long int d = millis();
        //MPLog("[%lu] A2:%.2lf,A3:%.2lf\r\n", t_ms, measured_pressure[0], measured_pressure[1]);
        //MPLog("A2:%.2lf,A3:%.2lf,A4:%.2lf,A5:%.2lf\r\n", measured_pressure[0], measured_pressure[1], measured_pressure[2], measured_pressure[3]);
        sendPacket();
        return INTERVAL_US;
    }
    
    void setup(){
        pinMode(PIN_VALVE_1, OUTPUT);
        pinMode(PIN_VALVE_2, OUTPUT);
        pinMode(PIN_VALVE_3, OUTPUT);
        pinMode(PIN_VALVE_4, OUTPUT);
        digitalWrite(PIN_VALVE_1, 0);
        digitalWrite(PIN_VALVE_2, 0);
        digitalWrite(PIN_VALVE_3, 0);
        digitalWrite(PIN_VALVE_4, 0);
        MCP4726WritePressure(0);
        Serial.begin(115200);
        attachTimerInterrupt(flip, INTERVAL_US);
    }

    void loop(){
      regulator_sense_value = analogRead(PIN_REGULATOR_SENSE);
      flow_sensor_value = analogRead(PIN_FLOW_SENSOR);
      pressure_sensor_value[0] = analogRead(PIN_PRESSURE_SENSOR_1);
      pressure_sensor_value[1] = analogRead(PIN_PRESSURE_SENSOR_2);
      pressure_sensor_value[2] = analogRead(PIN_PRESSURE_SENSOR_3);
      digitalWrite(PIN_VALVE_1, target_valve_state&0x01);
      digitalWrite(PIN_VALVE_2, (target_valve_state>>1)&0x01);
      digitalWrite(PIN_VALVE_3, (target_valve_state>>2)&0x01);
      digitalWrite(PIN_VALVE_4, (target_valve_state>>3)&0x01);
      MCP4726WritePressure(target_regulator_pressure);
      // for (int i = 0; i<NUMBER_OF_SENSOR; i++){
      //   measured_pressure[i] = (5.0*val[i]/1024-1.0)/4.0*1000.0;
      //   //measured_pressure[i] = 1.0*val[i];
      
      // unsigned long int d = millis();
      //int value = analogRead(A2);
      //MPLog("%lf\r\n", 5.0*value/1024);
      //delay(200);
    }

    void sendPacket(){
      Serial.write(SEND_START_CODE);
      dataSendEach(regulator_sense_value);
      dataSendEach(flow_sensor_value);
      dataSendEach(pressure_sensor_value[0]);
      dataSendEach(pressure_sensor_value[1]);
      dataSendEach(pressure_sensor_value[2]);
      Serial.write('\r');
      Serial.write('\n');
    }

    void dataSendEach(int16_t data){
      uint8_t high_side_data, low_side_data;
      data2Packet(&high_side_data, &low_side_data, data);
      Serial.write(high_side_data);
      Serial.write(low_side_data);
      Serial.write(0x20);
    }

    int16_t packet2Data(uint8_t high_side_data, uint8_t low_side_data){
      return ((int16_t)high_side_data-0x86)*100 + ((int16_t)low_side_data-0x86);
    }

    void data2Packet(uint8_t* high_side_ptr, uint8_t* low_side_ptr, int16_t data){
      *high_side_ptr = (uint8_t)(data/100 + 0x86);
      *low_side_ptr = (uint8_t)(data%100 + 0x86);
    }
