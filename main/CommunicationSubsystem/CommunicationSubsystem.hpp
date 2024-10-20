#ifndef COMMUNICATION_SUBSYSTEM_HPP
#define COMMUNICATION_SUBSYSTEM_HPP

#include "Ethernet.hpp"
#include "MQTT.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class CommunicationSubsystem {
public:
    CommunicationSubsystem(int sending_period = 20);
    ~CommunicationSubsystem();

    void init();
    void startCommunication();
    void stopCommunication();
    static void communicationRoutine(void *pvParameters);

private:
    int sending_period; // ms
    TaskHandle_t communication_thread_handle;
    MQTT mqtt = MQTT(10, 900);
};

#endif // COMMUNICATION_SUBSYSTEM_HPP