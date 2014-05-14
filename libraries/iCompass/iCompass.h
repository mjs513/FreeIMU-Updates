#ifndef iCompass_h
#define iCompass_h

#include <Arduino.h> // for byte data type

class iCompass
{
  public:
    template <typename T> struct vector
    {
      T x, y, z;
    };


    vector<float> a; // accelerometer readings
    vector<float> m; // magnetometer readings

    iCompass(void);

    //float heading(void);
    float iheading(int ix, int iy, int iz, float ax, float ay, float az, float mx, float my, float mz);

    // vector functions
    template <typename Ta, typename Tb, typename To> static void vector_cross(const vector<Ta> *a, const vector<Tb> *b, vector<To> *out);
    template <typename Ta, typename Tb> static float vector_dot(const vector<Ta> *a,const vector<Tb> *b);
    static void vector_normalize(vector<float> *a);


};

#endif



