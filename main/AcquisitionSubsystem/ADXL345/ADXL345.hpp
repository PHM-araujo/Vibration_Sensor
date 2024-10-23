#ifndef ADXL345_HPP
#define ADXL345_HPP

#include "Adafruit_ADXL345_U.h"
#include <Adafruit_Sensor.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"


#define ADXL345_SDA 33
#define ADXL345_SCL 32
#define ADXL345_INT2 35

class ADXL345: public Adafruit_ADXL345_Unified
{  
public:
    enum Axis {X, Y, Z};

    ADXL345(int32_t sensorID = 12345);
    void displayRange();
    void displayDataRate();
    void displayAccelerations(int period);
    void calibrateAxis(int16_t num_readings = 250);
    void setOffset(int16_t offset, Axis eixo);
    void init();
    void displayAccelerationUnscaled(int period);
    void GetAccelerations(float* x, float* y, float* z);
};

#endif // ADXL345_HPP