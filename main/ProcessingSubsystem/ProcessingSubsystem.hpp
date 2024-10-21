#ifndef PROCESSING_SUBSYSTEM_HPP
#define PROCESSING_SUBSYSTEM_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "LowPassFilter.hpp"

class ProcessingSubsystem {
public:
    ProcessingSubsystem();
    ~ProcessingSubsystem();

    void init(int processing_period);
    void startProcessing();
    void stopProcessing();
    static void processingRoutine(void *pvParameters);

private:
    int _processing_period; // ms
    LowPassFilter _low_pass_filter_x;
    LowPassFilter _low_pass_filter_y;
    LowPassFilter _low_pass_filter_z;
    TaskHandle_t processing_thread_handle;
};

#endif // COMMUNICATION_SUBSYSTEM_HPP