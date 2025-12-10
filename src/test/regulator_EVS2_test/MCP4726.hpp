
  #ifndef _MCP4726_HPP_
  #define _MCP4726_HPP_
  
  #include<Wire.h>
  const byte MCP4726A0_ADDR = 0x60;
  const int MCP4726_DAC_RESOLUTION = 4096;
  const int P_MAX_kPa = 500;
  const double VDD = 5.0;

  void MCP4726WriteVolatile(byte power_down, uint16_t data){
    Wire.beginTransmission(MCP4726A0_ADDR);
    Wire.write(0x00<<6 | (0x03&power_down)<<4 | (byte)(0x000F&(data>>8)));
    Wire.write((byte)(0x00FF&data));
    Wire.endTransmission();
  }

  void MCP4726WriteVal(uint16_t val){
    MCP4726WriteVolatile(0x00, val);  // プルダウンなし
  }

  void MCP4726WriteVoltage(double voltage){
    uint16_t val = voltage/VDD*4096;
    MCP4726WriteVal(val);
  }

  void MCP4726WritePressure(int pressure_kPa){
    MCP4726WriteVoltage((double)pressure_kPa/P_MAX_kPa*(5-1)+1);  // 計算の詳細はEVS2のデータシート参照
  }

  void MCP4726init(){
    Wire.begin();
  }

  #endif
