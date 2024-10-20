#include "AcquisitionSubsystem.hpp"
#include "BlockingQueue.hpp"

AcquisitionSubsystem::AcquisitionSubsystem(int sampling_period) 
    : accelerometer(12345),
    sampling_period(sampling_period),
    acquisition_task_handle(nullptr) {}

void AcquisitionSubsystem::init() {
    accelerometer.init();
    accelerometer.calibrateAxis();
    accelerometer.enableActivityDetection();
}

void AcquisitionSubsystem::startAcquisition() {
    if (acquisition_task_handle != nullptr) {
        vTaskDelete(acquisition_task_handle);
        acquisition_task_handle = nullptr;
    }
    xTaskCreate(
        AcquisitionSubsystem::AcquisitionRoutine, 
        "AcquisitionRoutine", 
        4096, 
        this, 
        5, 
        &acquisition_task_handle
    );
}

void AcquisitionSubsystem::AcquisitionRoutine(void *pvParameters) {
    AcquisitionSubsystem *acquisition_subsystem = (AcquisitionSubsystem *)pvParameters;
    while (true) {
        float x, y, z;
        acquisition_subsystem->accelerometer.GetAccelerations(&x, &y, &z);
        AccelerometerData data = {x, y, z};
        ESP_LOGI("AcquisitionSubsystem", "Acquiring data: %f %f %f", data.acceleration_x, data.acceleration_y, data.acceleration_z);
        processing_queue.push(data);
        vTaskDelay(acquisition_subsystem->sampling_period / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void AcquisitionSubsystem::stopAcquisition() {
    if (acquisition_task_handle != nullptr) {
        vTaskDelete(acquisition_task_handle);
        acquisition_task_handle = nullptr;
    }
}



bool AcquisitionSubsystem::getIsMoving() {
    return accelerometer.getIsActivity();
}