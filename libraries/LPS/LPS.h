#ifndef LPS_h
#define LPS_h

#include <Arduino.h> // for byte data type

class LPS
{
  public:
    enum deviceType { device_331AP, device_25H, device_auto };
    enum sa0State { sa0_low, sa0_high, sa0_auto };

    // register addresses
    // Note: where register names differ between the register mapping table and
    // the register descriptions in the datasheets, the names from the register
    // descriptions are used here.
    enum regAddr
    {
      REF_P_XL                = 0x08,
      REF_P_L                 = 0x09,
      REF_P_H                 = 0x0A,
                              
      WHO_AM_I                = 0x0F,
                              
      RES_CONF                = 0x10,
                              
      CTRL_REG1               = 0x20,
      CTRL_REG2               = 0x21,
      CTRL_REG3               = 0x22,
      CTRL_REG4               = 0x23, // 25H
              
      STATUS_REG              = 0x27,
                            
      PRESS_OUT_XL            = 0x28,
      PRESS_OUT_L             = 0x29,
      PRESS_OUT_H             = 0x2A,

      TEMP_OUT_L              = 0x2B,
      TEMP_OUT_H              = 0x2C,
      
      FIFO_CTRL               = 0x2E, // 25H
      FIFO_STATUS             = 0x2F, // 25H
      
      AMP_CTRL                = 0x30, // 331AP
      
      RPDS_L                  = 0x39, // 25H
      RPDS_H                  = 0x3A, // 25H
      
      DELTA_PRESS_XL          = 0x3C, // 331AP
      DELTA_PRESS_L           = 0x3D, // 331AP
      DELTA_PRESS_H           = 0x3E, // 331AP
      
      
      // dummy addresses for registers in different locations on different devices;
      // the library translates these based on device type
      // value with sign flipped is used as index into translated_regs array
    
      INTERRUPT_CFG    = -1,
      INT_SOURCE       = -2,
      THS_P_L          = -3,
      THS_P_H          = -4,
      // update dummy_reg_count if registers are added here!
      
      
      // device-specific register addresses
      
      LPS331AP_INTERRUPT_CFG  = 0x23,
      LPS331AP_INT_SOURCE     = 0x24,
      LPS331AP_THS_P_L        = 0x25,
      LPS331AP_THS_P_H        = 0x26,
      
      LPS25H_INTERRUPT_CFG    = 0x24,
      LPS25H_INT_SOURCE       = 0x25,
      LPS25H_THS_P_L          = 0x30,
      LPS25H_THS_P_H          = 0x31,
    };

    LPS(void);

    bool init(deviceType device = device_auto, byte sa0 = sa0_auto);
    deviceType getDeviceType(void) { return _device; }
    byte getAddress(void) { return address; }

    void enableDefault(void);

    void writeReg(int reg, byte value);
    byte readReg(int reg);

    float readPressureMillibars(void);
    float readPressureInchesHg(void);
    int32_t readPressureRaw(void);
    float readTemperatureC(void);
    float readTemperatureF(void);
    int16_t readTemperatureRaw(void);

    static float pressureToAltitudeMeters(float pressure_mbar, float altimeter_setting_mbar = 1013.25);
    static float pressureToAltitudeFeet(float pressure_inHg, float altimeter_setting_inHg = 29.9213);

  private:
    deviceType _device; // chip type (331AP or 25H)
    byte address;
    
    static const int dummy_reg_count = 4;
    regAddr translated_regs[dummy_reg_count + 1]; // index 0 not used

    bool detectDeviceAndAddress(deviceType device, sa0State sa0);
    bool detectDevice(deviceType device);
    int testWhoAmI(byte address);
};

#endif