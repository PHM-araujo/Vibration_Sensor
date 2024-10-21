#include "Arduino.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include "VibrationSystem.hpp"
#include "StartingDevice.hpp"

extern "C" void app_main()
{
    VibrationSystem vibration_system;
    vibration_system.init(new StartingDevice());

    while (true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}