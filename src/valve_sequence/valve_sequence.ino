#include <MP.h>  // MPLog用

#define PIN_VALVE_1 PIN_D02
#define PIN_VALVE_2 PIN_D03
#define PIN_VALVE_3 PIN_D04
#define PIN_VALVE_4 PIN_D05

#define INTERVAL_TIME_MS 500
#define ACTIVE_TIME_MS 500

#define SEQUENCE_MODE_STOP 0
#define SEQUENCE_MODE_ON_OFF 1
#define SEQUENCE_MODE_LOGIC 2
#define SEQUENCE_MODE_LATCH 3
#define SEQUENCE_MODE_WAVY_THREE 4

void setValvesState(int v1 = 0, int v2 = 0, int v3 = 0, int v4 = 0){
  digitalWrite(PIN_VALVE_1, v1);
  digitalWrite(PIN_VALVE_2, v2);
  digitalWrite(PIN_VALVE_3, v3);
  digitalWrite(PIN_VALVE_4, v4);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_VALVE_1, OUTPUT);
  pinMode(PIN_VALVE_2, OUTPUT);
  pinMode(PIN_VALVE_3, OUTPUT);
  pinMode(PIN_VALVE_4, OUTPUT);
  setValvesState(0,0,0,0);
  Serial.begin(115200);
  delay(300);
}

void loop() {
  // put your main code here, to run repeatedly:

  static int sequence_cnt = 0;
  static int sequence_mode = SEQUENCE_MODE_STOP;

  if (sequence_mode == SEQUENCE_MODE_STOP){
    setValvesState(0,0,0,0);
  }

  if (sequence_mode == SEQUENCE_MODE_ON_OFF){
    setValvesState(sequence_cnt == 0,sequence_cnt == 1);
    sequence_cnt = (int)(sequence_cnt == 0);
  }
  
  if (sequence_mode == SEQUENCE_MODE_LOGIC){
    if (sequence_cnt < 1){
      setValvesState(0,0);
    }else if (sequence_cnt < 3){
      setValvesState(1,0);
    }else if (sequence_cnt < 4){
      setValvesState(0,0);
    }else if (sequence_cnt < 6){
      setValvesState(0,1);
    }else if (sequence_cnt < 7){
      setValvesState(0,0);
    }else if (sequence_cnt < 9){
      setValvesState(1,1);
    }else if (sequence_cnt < 10){
      setValvesState(0,0);
    }else{
      sequence_cnt = 0;
    }
    sequence_cnt++;
  }

  if (sequence_mode == SEQUENCE_MODE_LATCH){
    if (sequence_cnt < 1){
      setValvesState(0,0);
    }else if (sequence_cnt < 3){
      setValvesState(1,0);
    }else if (sequence_cnt < 4){
      setValvesState(0,0);
    }else if (sequence_cnt < 6){
      setValvesState(1,0);
    }else if (sequence_cnt < 7){
      setValvesState(0,0);
    }else if (sequence_cnt < 9){
      setValvesState(0,1);
    }else if (sequence_cnt < 10){
      setValvesState(0,0);
    }else{
      sequence_cnt = 0;
    }
    sequence_cnt++;
  }

   if (sequence_mode == SEQUENCE_MODE_WAVY_THREE){
     if (sequence_cnt <= 1){
       setValvesState(0,1,1);
     }else if (sequence_cnt <= 2){
       setValvesState(1,1,0);
     }else if (sequence_cnt <= 3){
       setValvesState(1,0,1);
       sequence_cnt = 0;
     }
    // if (sequence_cnt < 1){
    //   setValvesState(0,0,1);
    // }else if (sequence_cnt < 2){
    //   setValvesState(0,1,1);
    // }else if (sequence_cnt < 3){
    //   setValvesState(1,1,0);
    // }else if (sequence_cnt < 4){
    //   setValvesState(1,0,0);
    // }else if (sequence_cnt < 5){
    //   setValvesState(1,0,1);
    // }else{
    //   sequence_cnt = 0;
    // }
    sequence_cnt++;
  }

  if (Serial.available() > 0){
    int receiving_byte = Serial.read();
    if (receiving_byte == '1'){
      sequence_mode = SEQUENCE_MODE_ON_OFF;
    }
    if (receiving_byte == '2'){
      sequence_mode = SEQUENCE_MODE_LOGIC;
    }
    if (receiving_byte == '3'){
      sequence_mode = SEQUENCE_MODE_LATCH;
    }
    if (receiving_byte == '4'){
      sequence_mode = SEQUENCE_MODE_WAVY_THREE;
    }
    if (receiving_byte == 'q'){
      sequence_mode = SEQUENCE_MODE_STOP;
    }
    sequence_cnt = 0;
  }
  delay(INTERVAL_TIME_MS);
}
