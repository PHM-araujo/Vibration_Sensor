#include "ProcessingSubsystem.hpp"
#include "BlockingQueue.hpp"
#include <esp_log.h>
#include <cmath>

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
    _low_pass_filter_x.reset();
    _low_pass_filter_y.reset();
    _low_pass_filter_z.reset();
    sending_queue.clear();
}

void ProcessingSubsystem::processingRoutine(void *pvParameters) {
    ProcessingSubsystem *processing_subsystem = (ProcessingSubsystem *)pvParameters;
    while (true) {
        AccelerometerData data = processing_queue.pop();
        data.acceleration_x = processing_subsystem->_low_pass_filter_x.filt(data.acceleration_x);
        data.acceleration_y = processing_subsystem->_low_pass_filter_y.filt(data.acceleration_y);
        data.acceleration_z = processing_subsystem->_low_pass_filter_z.filt(data.acceleration_z);
        if (std::isnan(data.acceleration_x) || std::isnan(data.acceleration_y) || std::isnan(data.acceleration_z)) {
            ESP_LOGE("ProcessingSubsystem", "Invalid acceleration values");
            continue;
        }
        ProcessedData processed_data = {data.acceleration_x, data.acceleration_y, data.acceleration_z};
        sending_queue.push(processed_data);
        vTaskDelay(processing_subsystem->_processing_period / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}