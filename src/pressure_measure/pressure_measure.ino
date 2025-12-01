#include <MP.h>  // MPLog用
    #define INTERVAL_US 10000
    const int NUMBER_OF_SENSOR = 4;
    double measured_pressure[NUMBER_OF_SENSOR];
    unsigned long int t_ms = 0;
    
    unsigned int flip(){
        //unsigned long int d = millis();
        //MPLog("[%lu] A2:%.2lf,A3:%.2lf\r\n", t_ms, measured_pressure[0], measured_pressure[1]);
        MPLog("A2:%.2lf,A3:%.2lf,A4:%.2lf,A5:%.2lf\r\n", measured_pressure[0], measured_pressure[1], measured_pressure[2], measured_pressure[3]);
        return INTERVAL_US;
    }
    
    void setup(){
        attachTimerInterrupt(flip, INTERVAL_US);

    }

    void loop(){
      // MPLog("hallo: %lu\r\n", d);
      t_ms = 0;//millis();
      int val[NUMBER_OF_SENSOR];
      val[0] = analogRead(A2);
      val[1] = analogRead(A3);
      val[2] = analogRead(A4);
      val[3] = analogRead(A5);

      for (int i = 0; i<NUMBER_OF_SENSOR; i++){
        measured_pressure[i] = (5.0*val[i]/1024-1.0)/4.0*1000.0;
        //measured_pressure[i] = 1.0*val[i];
      }
      // unsigned long int d = millis();
      //int value = analogRead(A2);
      //MPLog("%lf\r\n", 5.0*value/1024);
      //delay(200);
    }