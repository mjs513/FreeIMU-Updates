#ifndef LSM303_h
#define LSM303_h

#include <Arduino.h> // for byte data type

class LSM303
{
  public:
    template <typename T> struct vector
    {
      T x, y, z;
    };

    enum deviceType { device_DLH, device_DLM, device_DLHC, device_D, device_auto };
    enum sa0State { sa0_low, sa0_high, sa0_auto };

    // register addresses
    enum regAddr
    {
      TEMP_OUT_L        = 0x05, // D
      TEMP_OUT_H        = 0x06, // D

      STATUS_M          = 0x07, // D

      INT_CTRL_M        = 0x12, // D
      INT_SRC_M         = 0x13, // D
      INT_THS_L_M       = 0x14, // D
      INT_THS_H_M       = 0x15, // D

      OFFSET_X_L_M      = 0x16, // D
      OFFSET_X_H_M      = 0x17, // D
      OFFSET_Y_L_M      = 0x18, // D
      OFFSET_Y_H_M      = 0x19, // D
      OFFSET_Z_L_M      = 0x1A, // D
      OFFSET_Z_H_M      = 0x1B, // D
      REFERENCE_X       = 0x1C, // D
      REFERENCE_Y       = 0x1D, // D
      REFERENCE_Z       = 0x1E, // D

      CTRL0             = 0x1F, // D
      CTRL1             = 0x20, // D
      CTRL_REG1_A       = 0x20, // DLH, DLM, DLHC
      CTRL2             = 0x21, // D
      CTRL_REG2_A       = 0x21, // DLH, DLM, DLHC
      CTRL3             = 0x22, // D
      CTRL_REG3_A       = 0x22, // DLH, DLM, DLHC
      CTRL4             = 0x23, // D
      CTRL_REG4_A       = 0x23, // DLH, DLM, DLHC
      CTRL5             = 0x24, // D
      CTRL_REG5_A       = 0x24, // DLH, DLM, DLHC
      CTRL6             = 0x25, // D
      CTRL_REG6_A       = 0x25, // DLHC
      HP_FILTER_RESET_A = 0x25, // DLH, DLM
      CTRL7             = 0x26, // D
      REFERENCE_A       = 0x26, // DLH, DLM, DLHC
      STATUS_A          = 0x27, // D
      STATUS_REG_A      = 0x27, // DLH, DLM, DLHC

      OUT_X_L_A         = 0x28,
      OUT_X_H_A         = 0x29,
      OUT_Y_L_A         = 0x2A,
      OUT_Y_H_A         = 0x2B,
      OUT_Z_L_A         = 0x2C,
      OUT_Z_H_A         = 0x2D,

      FIFO_CTRL         = 0x2E, // D
      FIFO_CTRL_REG_A   = 0x2E, // DLHC
      FIFO_SRC          = 0x2F, // D
      FIFO_SRC_REG_A    = 0x2F, // DLHC

      IG_CFG1           = 0x30, // D
      INT1_CFG_A        = 0x30, // DLH, DLM, DLHC
      IG_SRC1           = 0x31, // D
      INT1_SRC_A        = 0x31, // DLH, DLM, DLHC
      IG_THS1           = 0x32, // D
      INT1_THS_A        = 0x32, // DLH, DLM, DLHC
      IG_DUR1           = 0x33, // D
      INT1_DURATION_A   = 0x33, // DLH, DLM, DLHC
      IG_CFG2           = 0x34, // D
      INT2_CFG_A        = 0x34, // DLH, DLM, DLHC
      IG_SRC2           = 0x35, // D
      INT2_SRC_A        = 0x35, // DLH, DLM, DLHC
      IG_THS2           = 0x36, // D
      INT2_THS_A        = 0x36, // DLH, DLM, DLHC
      IG_DUR2           = 0x37, // D
      INT2_DURATION_A   = 0x37, // DLH, DLM, DLHC

      CLICK_CFG         = 0x38, // D
      CLICK_CFG_A       = 0x38, // DLHC
      CLICK_SRC         = 0x39, // D
      CLICK_SRC_A       = 0x39, // DLHC
      CLICK_THS         = 0x3A, // D
      CLICK_THS_A       = 0x3A, // DLHC
      TIME_LIMIT        = 0x3B, // D
      TIME_LIMIT_A      = 0x3B, // DLHC
      TIME_LATENCY      = 0x3C, // D
      TIME_LATENCY_A    = 0x3C, // DLHC
      TIME_WINDOW       = 0x3D, // D
      TIME_WINDOW_A     = 0x3D, // DLHC

      Act_THS           = 0x3E, // D
      Act_DUR           = 0x3F, // D

      CRA_REG_M         = 0x00, // DLH, DLM, DLHC
      CRB_REG_M         = 0x01, // DLH, DLM, DLHC
      MR_REG_M          = 0x02, // DLH, DLM, DLHC

      SR_REG_M          = 0x09, // DLH, DLM, DLHC
      IRA_REG_M         = 0x0A, // DLH, DLM, DLHC
      IRB_REG_M         = 0x0B, // DLH, DLM, DLHC
      IRC_REG_M         = 0x0C, // DLH, DLM, DLHC

      WHO_AM_I_M        = 0x0F, // DLM
      WHO_AM_I          = 0x0F, // D

      TEMP_OUT_H_M      = 0x31, // DLHC
      TEMP_OUT_L_M      = 0x32, // DLHC


      // dummy addresses for registers in different locations on different devices;
      // the library translates these based on device type
      // value with sign flipped is used as index into translated_regs array

      OUT_X_H_M         = -1,
      OUT_X_L_M         = -2,
      OUT_Y_H_M         = -3,
      OUT_Y_L_M         = -4,
      OUT_Z_H_M         = -5,
      OUT_Z_L_M         = -6,
      // update dummy_reg_count if registers are added here!

      // device-specific register addresses

      DLH_OUT_X_H_M     = 0x03,
      DLH_OUT_X_L_M     = 0x04,
      DLH_OUT_Y_H_M     = 0x05,
      DLH_OUT_Y_L_M     = 0x06,
      DLH_OUT_Z_H_M     = 0x07,
      DLH_OUT_Z_L_M     = 0x08,

      DLM_OUT_X_H_M     = 0x03,
      DLM_OUT_X_L_M     = 0x04,
      DLM_OUT_Z_H_M     = 0x05,
      DLM_OUT_Z_L_M     = 0x06,
      DLM_OUT_Y_H_M     = 0x07,
      DLM_OUT_Y_L_M     = 0x08,

      DLHC_OUT_X_H_M    = 0x03,
      DLHC_OUT_X_L_M    = 0x04,
      DLHC_OUT_Z_H_M    = 0x05,
      DLHC_OUT_Z_L_M    = 0x06,
      DLHC_OUT_Y_H_M    = 0x07,
      DLHC_OUT_Y_L_M    = 0x08,

      D_OUT_X_L_M       = 0x08,
      D_OUT_X_H_M       = 0x09,
      D_OUT_Y_L_M       = 0x0A,
      D_OUT_Y_H_M       = 0x0B,
      D_OUT_Z_L_M       = 0x0C,
      D_OUT_Z_H_M       = 0x0D
    };

    vector<int16_t> a; // accelerometer readings
    vector<int16_t> m; // magnetometer readings
    vector<int16_t> m_max; // maximum magnetometer values, used for calibration
    vector<int16_t> m_min; // minimum magnetometer values, used for calibration

    byte last_status; // status of last I2C transmission

    LSM303(void);

    bool init(deviceType device = device_auto, sa0State sa0 = sa0_auto);
    byte getDeviceType(void) { return _device; }

    void enableDefault(void);

    void writeAccReg(regAddr reg, byte value);
    byte readAccReg(regAddr reg);
    void writeMagReg(regAddr reg, byte value);
    byte readMagReg(regAddr reg);

    void writeReg(regAddr reg, byte value);
    byte readReg(regAddr reg);

    void readAcc(void);
    void readMag(void);
    void read(void);

    void setTimeout(unsigned int timeout);
    unsigned int getTimeout(void);
    bool timeoutOccurred(void);

    float heading(void);
    template <typename T> float heading(vector<T> from);

    // vector functions
    template <typename Ta, typename Tb, typename To> static void vector_cross(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out);
    template <typename Ta, typename Tb> static float vector_dot(const vector<Ta> *a,const vector<Tb> *b);
    static void vector_normalize(vector<float> *a);

  private:
    deviceType _device; // chip type (DLH, DLM, or DLHC)
    byte acc_address;
    byte mag_address;

    static const int dummy_reg_count = 6;
    regAddr translated_regs[dummy_reg_count + 1]; // index 0 not used

    unsigned int io_timeout;
    bool did_timeout;

    int testReg(byte address, regAddr reg);
};

#endif



