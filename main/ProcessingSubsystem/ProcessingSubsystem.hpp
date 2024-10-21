#ifndef PROCESSING_SUBSYSTEM_HPP
#define PROCESSING_SUBSYSTEM_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

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
    TaskHandle_t processing_thread_handle;
};

#endif // COMMUNICATION_SUBSYSTEM_HPP