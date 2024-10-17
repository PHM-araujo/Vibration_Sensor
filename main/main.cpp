#include "Arduino.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include "AcquisitionSubsystem.hpp"

extern "C" void app_main()
{
    AcquisitionSubsystem acquisition_subsystem;
    acquisition_subsystem.init();
    int lock = 0;
    while (true)
    {
        ESP_LOGI("AcquisitionSubsystem", "Checking if device is moving");
        if(acquisition_subsystem.getIsMoving()){
            if (lock == 0){
                ESP_LOGI("AcquisitionSubsystem", "Device is moving");
                acquisition_subsystem.startAcquisition();
                lock = 1;
            }
        }
        else{
            lock = 0;
            acquisition_subsystem.stopAcquisition();
            ESP_LOGI("AcquisitionSubsystem", "Device is not moving");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
}