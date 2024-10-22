#include "AcquisitionSubsystem.hpp"
#include "BlockingQueue.hpp"

bool AcquisitionSubsystem::is_active = false;
TimerHandle_t AcquisitionSubsystem::xTimerActive = nullptr;
TimerHandle_t AcquisitionSubsystem::xTimerInactive = nullptr;
uint8_t AcquisitionSubsystem::count_inactive = 0;
bool AcquisitionSubsystem::is_timer_alive = false;

AcquisitionSubsystem::AcquisitionSubsystem() 
    : accelerometer(12345),
    _sampling_period(2),
    acquisition_task_handle(nullptr) {}

AcquisitionSubsystem::~AcquisitionSubsystem() {
    if (acquisition_task_handle != nullptr) {
        vTaskDelete(acquisition_task_handle);
    }
    if (xTimerActive != nullptr) {
        xTimerDelete(xTimerActive, 0);
    }
    if (xTimerInactive != nullptr) {
        xTimerDelete(xTimerInactive, 0);
    }
}

void AcquisitionSubsystem::init(int sampling_period) {
    accelerometer.init();
    _sampling_period = sampling_period;
    accelerometer.calibrateAxis();
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
        processing_queue.push(data);
        vTaskDelay(acquisition_subsystem->_sampling_period / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void AcquisitionSubsystem::stopAcquisition() {
    if (acquisition_task_handle != nullptr) {
        vTaskDelete(acquisition_task_handle);
        acquisition_task_handle = nullptr;
    }
}

void AcquisitionSubsystem::setActiveThreshold(float threshold) {
    thresholdActive = threshold;
}

void AcquisitionSubsystem::setInactiveThreshold(float threshold) {
    thresholdInactive = threshold;
}

void AcquisitionSubsystem::enableActivityDetection(float threshold, int period) {
    is_active = false;
    is_timer_alive = false;
    setActiveThreshold(threshold);
    startPollingTimer(period, &xTimerActive, timerCallbackActive);
}

void AcquisitionSubsystem::enableInactivityDetection(float threshold, int period) {
    is_active = true;
    setInactiveThreshold(threshold);
    startPollingTimer(period, &xTimerInactive, timerCallbackInactive);
}

void AcquisitionSubsystem::startPollingTimer(int period, TimerHandle_t* xTimer, TimerCallbackFunction_t timer_callback) {
    if (*xTimer == nullptr) {
        *xTimer = xTimerCreate("ActivePollingTimer", pdMS_TO_TICKS(period), pdTRUE, (void*)this, timer_callback);
    } else {
        xTimerChangePeriod(*xTimer, pdMS_TO_TICKS(period), 0);
    }
    if (xTimer != nullptr) {
        xTimerStart(*xTimer, 0);
    } else {
        ESP_LOGI("AcquisitionSubsystem", "Failed to create timer");
    }
}

void AcquisitionSubsystem::timerCallbackActive(TimerHandle_t xTimer) {
    AcquisitionSubsystem *acquisition_subsystem = (AcquisitionSubsystem *)pvTimerGetTimerID(xTimer);
    float accel_x, accel_y, accel_z;
    acquisition_subsystem->accelerometer.GetAccelerations(&accel_x, &accel_y, &accel_z);
    ESP_LOGI("ADXL345", "Active");
    if (abs(accel_x) > acquisition_subsystem->thresholdActive || abs(accel_y) > acquisition_subsystem->thresholdActive) {
        is_active = true;
    } else {
        is_active = false;
    }
    is_timer_alive = true;
}

void AcquisitionSubsystem::timerCallbackInactive(TimerHandle_t xTimer) {
    AcquisitionSubsystem *acquisition_subsystem = (AcquisitionSubsystem *)pvTimerGetTimerID(xTimer);
    float accel_x, accel_y, accel_z;
    acquisition_subsystem->accelerometer.GetAccelerations(&accel_x, &accel_y, &accel_z);
    if (abs(accel_x) < acquisition_subsystem->thresholdInactive and abs(accel_y) < acquisition_subsystem->thresholdInactive) {
        count_inactive++;
    } else {
        count_inactive = 0;
    }
    if (count_inactive > 5) {
        is_active = false;
    }
}

bool AcquisitionSubsystem::getIsMoving() {
    return is_active;
}

void AcquisitionSubsystem::disableActivityDetection() {
    if (xTimerActive != nullptr) {
        xTimerStop(xTimerActive, 0);
        xTimerDelete(xTimerActive, 0);
        xTimerActive = nullptr;
        ESP_LOGI("ADXL345", "Activity detection disabled");
    }
}

void AcquisitionSubsystem::disableInactivityDetection() {
    if (xTimerInactive != nullptr) {
        xTimerStop(xTimerInactive, 0);
        xTimerDelete(xTimerInactive, 0);
        xTimerInactive = nullptr;
        count_inactive = 0;
        ESP_LOGI("ADXL345", "Inactivity detection disabled");
    }
}

bool AcquisitionSubsystem::getTimerIsAlive() {
    return is_timer_alive;
}
