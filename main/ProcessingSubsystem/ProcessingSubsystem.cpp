#include "ProcessingSubsystem.hpp"
#include "BlockingQueue.hpp"
#include <esp_log.h>

ProcessingSubsystem::ProcessingSubsystem()
    : _processing_period(2),
    processing_thread_handle(nullptr) {}

ProcessingSubsystem::~ProcessingSubsystem() {
    stopProcessing();
}

void ProcessingSubsystem::init(int processing_period) {
    _processing_period = processing_period;
}

void ProcessingSubsystem::startProcessing() {
    if (processing_thread_handle != nullptr) {
        vTaskDelete(processing_thread_handle);
        processing_thread_handle = nullptr;
    }
    xTaskCreate(
        ProcessingSubsystem::processingRoutine, 
        "ProcessingRoutine", 
        4096, 
        this, 
        5, 
        &processing_thread_handle
    );
}

void ProcessingSubsystem::stopProcessing() {
    if (processing_thread_handle != nullptr) {
        vTaskDelete(processing_thread_handle);
        processing_thread_handle = nullptr;
    }
}

void ProcessingSubsystem::processingRoutine(void *pvParameters) {
    ProcessingSubsystem *processing_subsystem = (ProcessingSubsystem *)pvParameters;
    while (true) {
        AccelerometerData data = processing_queue.pop();
        // TODO: Aplicar filtro
        ProcessedData processed_data = {data.acceleration_x, data.acceleration_y, data.acceleration_z};
        sending_queue.push(processed_data);
        vTaskDelay(processing_subsystem->_processing_period / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}