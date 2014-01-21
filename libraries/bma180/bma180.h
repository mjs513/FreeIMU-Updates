
#ifndef _BMA180_h
#define _BMA180_h

#include <Wire.h>
/*
extern "C" {
#include <../Wire/Wire.h>

}
*/

#define BMA180_ADDRESS_SDO_LOW 0x40
#define BMA180_ADDRESS_SDO_HIGH 0x41

class BMA180
{
  public:
    typedef enum {F10HZ=0,F20HZ=1,F40HZ, F75HZ,F15HZ0,F300HZ,F600HZ,F1200HZ,HIGHPASS,BANDPASS} FILTER;
    typedef enum {G1=0,G15=1,G2,G3,G4,G8,G16}GSENSITIVITY;
private:
    unsigned char address;
    GSENSITIVITY gSense;
  public:

    int x,y,z; // yes, public, what the heck
    int temp;

    BMA180();
    
    void setAddress(int val);
    void readAccel(int * buff);
    void readAccel(int * x, int * y, int * z);

    float getGSense();
    float getXValFloat();
    float getYValFloat();
    float getZValFloat();
    void setRegValue(int regAdr, int val, int maskPreserve);
    int getRegValue(int adr);
    void setGSensitivty(GSENSITIVITY maxg);
    void SetFilter(FILTER f);
    void SetISRMode();
    void SoftReset();
    void SetSMPSkip();
    int getIDs(int *id, int *version);
    void enableWrite();
    void disableWrite();
    virtual bool checkResult(int result);
};


#endif //_BMA180_h
