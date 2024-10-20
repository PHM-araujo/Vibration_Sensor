#include "Arduino.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include "AcquisitionSubsystem.hpp"
#include "CommunicationSubsystem.hpp"
#include "ProcessingSubsystem.hpp"

extern "C" void app_main()
{
    AcquisitionSubsystem acquisition_subsystem;
    CommunicationSubsystem communication_subsystem;
    ProcessingSubsystem processing_subsystem;

    acquisition_subsystem.init();
    communication_subsystem.init();

    acquisition_subsystem.startAcquisition();
    communication_subsystem.startCommunication();
    processing_subsystem.startProcessing();

    while (true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}