#ifndef ACQUISITION_SUBSYSTEM_HPP
#define ACQUISITION_SUBSYSTEM_HPP

#include "ADXL345.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class AcquisitionSubsystem {
public:
    AcquisitionSubsystem(int sampling_period = 2);

    void init();
    void startAcquisition();
    void stopAcquisition();
    static void AcquisitionRoutine(void *pvParameters);

    bool getIsMoving();

private:
    ADXL345 accelerometer;  // m/s^2
    int sampling_period; // ms
    TaskHandle_t acquisition_task_handle;
};

#endif // ACQUISITION_SUBSYSTEM_HPP