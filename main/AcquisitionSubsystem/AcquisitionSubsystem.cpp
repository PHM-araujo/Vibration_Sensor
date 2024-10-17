#include "AcquisitionSubsystem.hpp"
#include "BlockingQueue.hpp"

AcquisitionSubsystem::AcquisitionSubsystem(int sampling_period, int mean_window_size) 
    : accelerometer(12345), is_running(false), 
    sampling_period(sampling_period),
    mean_window_size(mean_window_size) {}

void AcquisitionSubsystem::init() {
    accelerometer.init();
    accelerometer.calibrateAxis();
    accelerometer.enableActivityDetection();
}

void AcquisitionSubsystem::startAcquisition() {
    is_running = true;
    acquisition_thread = std::thread(&AcquisitionSubsystem::AcquisitionRoutine, this);
}

void AcquisitionSubsystem::stopAcquisition() {
    is_running = false;
    if (acquisition_thread.joinable()) {
        acquisition_thread.join();
    }
}

void AcquisitionSubsystem::AcquisitionRoutine() {
    float sum_x = 0;
    int count = 0;
    float mean_acceleration_x = 0;
    float x;
    float velocity_x = 0;
    float velocity_x_old = 0;
    while (is_running) {
        accelerometer.GetAccelerationX(&x);
        sum_x += x;
        count++;
        if (count == mean_window_size) {
            ESP_LOGI("AcquisitionSubsystem", "Mean acceleration: %F", sum_x / mean_window_size);
            mean_acceleration_x = sum_x / mean_window_size;
            count = 0;
            sum_x = 0;
            velocity_x = velocity_x_old + (mean_acceleration_x * sampling_period);
            ESP_LOGI("AcquisitionSubsystem", "Velocity: %f", velocity_x);
            AccelerometerData data = {velocity_x, mean_acceleration_x};
            globalQueue.push(data);
        }
        vTaskDelay(sampling_period / portTICK_PERIOD_MS);
    }
}

bool AcquisitionSubsystem::getIsMoving() {
    return accelerometer.getIsActivity();
}