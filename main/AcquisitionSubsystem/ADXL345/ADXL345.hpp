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
    ~ADXL345();
    void displayRange();
    void displayDataRate();
    void displayAccelerations(int period);
    void calibrateAxis(int16_t num_readings = 500);
    void setOffset(int16_t offset, Axis eixo);
    void init();
    bool getIsActivity();
    void enableActivityDetection(float threshold = 2, int period = 500);
    void disableActivityDetection();
    void displayAccelerationUnscaled(int period);
    void setActiveThreshold(float threshold);
    void calculateVelocity(float deltaTime);
    void GetAccelerationX(float* x);
private:
    void startPollingTimer(int period, TimerHandle_t* xTimer, 
                            TimerCallbackFunction_t timer_callback);
    static void timerCallbackActive(TimerHandle_t xTimer);
    
    static TimerHandle_t xTimerActive;
    static bool is_active;
    float thresholdActive;
    float velocityX = 0;  
    float velocityY = 0; 
};

#endif // ADXL345_HPP