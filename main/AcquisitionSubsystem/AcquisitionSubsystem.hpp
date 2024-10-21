#ifndef ACQUISITION_SUBSYSTEM_HPP
#define ACQUISITION_SUBSYSTEM_HPP

#include "ADXL345.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

class AcquisitionSubsystem {
public:
    AcquisitionSubsystem();
    ~AcquisitionSubsystem();
    void init(int sampling_period);
    void startAcquisition();
    void stopAcquisition();
    static void AcquisitionRoutine(void *pvParameters);
    void enableActivityDetection(float threshold = 1, int period = 500);
    void enableInactivityDetection(float threshold = 1, int period = 500);
    bool getIsMoving();
    void disableActivityDetection();
    void disableInactivityDetection();
private:
    void setActiveThreshold(float threshold);
    void setInactiveThreshold(float threshold);
    void startPollingTimer(int period, TimerHandle_t* xTimer, 
                            TimerCallbackFunction_t timer_callback);
    static void timerCallbackActive(TimerHandle_t xTimer);
    static void timerCallbackInactive(TimerHandle_t xTimer);

    ADXL345 accelerometer;  // m/s^2
    int _sampling_period; // ms
    TaskHandle_t acquisition_task_handle;
    static TimerHandle_t xTimerActive;
    static TimerHandle_t xTimerInactive;
    static bool is_active;
    static uint8_t count_inactive;
    float thresholdActive;
    float thresholdInactive;
};

#endif // ACQUISITION_SUBSYSTEM_HPP