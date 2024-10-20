#include "ADXL345.hpp"
#include <esp_log.h>
#include <iostream>
#include <iomanip>

bool ADXL345::is_active = false;
TimerHandle_t ADXL345::xTimerActive = nullptr;

ADXL345::ADXL345(int32_t sensorID): Adafruit_ADXL345_Unified(sensorID){}

ADXL345::~ADXL345()
{
    disableActivityDetection();
}

void ADXL345::init()
{
    if (!Adafruit_ADXL345_Unified::begin(ADXL345_DEFAULT_ADDRESS))
    {
        ESP_LOGI("ADXL345", "Ooops, no ADXL345 detected ... Check your wiring!");
    } else {
        ESP_LOGI("ADXL345", "ADXL345 detected!");
    }

    setRange(ADXL345_RANGE_2_G);
    setDataRate(ADXL345_DATARATE_800_HZ);
}

void ADXL345::displayRange()
{
    range_t range = getRange();
    switch(range)
    {
        case ADXL345_RANGE_16_G:
            ESP_LOGI("ADXL345", "+/- 16G");
            break;
        case ADXL345_RANGE_8_G:
            ESP_LOGI("ADXL345", "+/- 8G");
            break;
        case ADXL345_RANGE_4_G:
            ESP_LOGI("ADXL345", "+/- 4G");
            break;
        case ADXL345_RANGE_2_G:
            ESP_LOGI("ADXL345", "+/- 2G");
            break;
    }
}

void ADXL345::displayDataRate()
{
    dataRate_t dataRate = getDataRate();
    switch(dataRate)
    {
        case ADXL345_DATARATE_3200_HZ:
            ESP_LOGI("ADXL345", "3200 Hz");
            break;
        case ADXL345_DATARATE_1600_HZ:
            ESP_LOGI("ADXL345", "1600 Hz");
            break;
        case ADXL345_DATARATE_800_HZ:
            ESP_LOGI("ADXL345", "800 Hz");
            break;
        case ADXL345_DATARATE_400_HZ:
            ESP_LOGI("ADXL345", "400 Hz");
            break;
        case ADXL345_DATARATE_200_HZ:
            ESP_LOGI("ADXL345", "200 Hz");
            break;
        case ADXL345_DATARATE_100_HZ:
            ESP_LOGI("ADXL345", "100 Hz");
            break;
        case ADXL345_DATARATE_50_HZ:
            ESP_LOGI("ADXL345", "50 Hz");
            break;
        case ADXL345_DATARATE_25_HZ:
            ESP_LOGI("ADXL345", "25 Hz");
            break;
        case ADXL345_DATARATE_12_5_HZ:
            ESP_LOGI("ADXL345", "12.5 Hz");
            break;
        case ADXL345_DATARATE_6_25HZ:
            ESP_LOGI("ADXL345", "6.25 Hz");
            break;
        case ADXL345_DATARATE_3_13_HZ:
            ESP_LOGI("ADXL345", "3.13 Hz");
            break;
        case ADXL345_DATARATE_1_56_HZ:
            ESP_LOGI("ADXL345", "1.56 Hz");
            break;
        case ADXL345_DATARATE_0_78_HZ:
            ESP_LOGI("ADXL345", "0.78 Hz");
            break;
        case ADXL345_DATARATE_0_39_HZ:
            ESP_LOGI("ADXL345", "0.39 Hz");
            break;
        case ADXL345_DATARATE_0_20_HZ:
            ESP_LOGI("ADXL345", "0.20 Hz");
            break;
        case ADXL345_DATARATE_0_10_HZ:
            ESP_LOGI("ADXL345", "0.10 Hz");
            break;
        default:
            ESP_LOGI("ADXL345", "??? Hz");
            break;
    }
}

void ADXL345::displayAccelerations(int period)
{
    sensors_event_t event;
    getEvent(&event);
    //ESP_LOGI("ADXL345", "X: %f m/s^2, Y: %f m/s^2, Z: %f m/s^2", event.acceleration.x, event.acceleration.y, event.acceleration.z);
    printf("%f %f %f\n", event.acceleration.x, event.acceleration.y, event.acceleration.z);
    vTaskDelay(period / portTICK_PERIOD_MS);
}

void ADXL345::displayAccelerationUnscaled(int period)
{
    printf("%d %d %d\n", getX(), getY(), getZ());
    vTaskDelay(period / portTICK_PERIOD_MS);
}

void ADXL345::calibrateAxis(int16_t num_readings)
{   
    int sum_z = 0;
    int sum_y = 0;
    int sum_x = 0;
    ESP_LOGI("ADXL345", "Calibrating axis");
    for (int i = 0; i < num_readings; i++)
    {
        sum_z += getZ();
        sum_y += getY();
        sum_x += getX();

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    int16_t offset = (256 - (sum_z / num_readings)) / 4;
    setOffset(offset, Z);
    offset = (0 - (sum_y / num_readings)) / 4;
    setOffset(offset, Y);
    offset = (0 - (sum_x / num_readings)) / 4;
    setOffset(offset, X);
}

void ADXL345::setOffset(int16_t offset, Axis axis)
{
    uint8_t reg = 0;
    switch(axis)
    {
        case X:
            reg = ADXL345_REG_OFSX;
            break;
        case Y:
            reg = ADXL345_REG_OFSY;
            break;
        case Z:
            reg = ADXL345_REG_OFSZ;
            break;
    }
    writeRegister(reg, offset);
}

void ADXL345::setActiveThreshold(float threshold) {
    thresholdActive = threshold;
}

void ADXL345::enableActivityDetection(float threshold, int period) {
    setActiveThreshold(threshold);
    startPollingTimer(period, &xTimerActive, timerCallbackActive);
}

void ADXL345::startPollingTimer(int period, TimerHandle_t* xTimer, TimerCallbackFunction_t timer_callback) {
    if (*xTimer == nullptr) {
        *xTimer = xTimerCreate("ActivePollingTimer", pdMS_TO_TICKS(period), pdTRUE, (void*)this, timer_callback);
    } else {
        xTimerChangePeriod(*xTimer, pdMS_TO_TICKS(period), 0);
    }
    if (xTimer != nullptr) {
        xTimerStart(*xTimer, 0);
    } else {
        ESP_LOGI("ADXL345", "Failed to create timer");
    }
}

void ADXL345::timerCallbackActive(TimerHandle_t xTimer) {
    ADXL345* sensor = static_cast<ADXL345*>(pvTimerGetTimerID(xTimer));
    sensors_event_t event;
    sensor->getEvent(&event);
    if (abs(event.acceleration.x) > sensor->thresholdActive || abs(event.acceleration.y) > sensor->thresholdActive) {
        is_active = true;
    } else {
        is_active = false;
    }
}

bool ADXL345::getIsActivity() {
    return is_active;
}

void ADXL345::disableActivityDetection() {
    if (xTimerActive != nullptr) {
        xTimerStop(xTimerActive, 0);
        xTimerDelete(xTimerActive, 0);
        xTimerActive = nullptr;
        is_active = false;
        ESP_LOGI("ADXL345", "Activity detection disabled");
    }
}

void ADXL345::GetAccelerations(float* x, float* y, float* z)
{
    sensors_event_t event;
    getEvent(&event);
    *x = event.acceleration.x;
    *y = event.acceleration.y;
    *z = event.acceleration.z;
}