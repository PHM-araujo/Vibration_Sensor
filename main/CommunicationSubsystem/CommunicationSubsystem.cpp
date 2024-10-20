#include "CommunicationSubsystem.hpp"
#include "BlockingQueue.hpp"
#include "definitions.h"
#include <esp_log.h>
#include <vector>
#include <sstream>
#include <iomanip>

CommunicationSubsystem::CommunicationSubsystem(int sending_period)
    : sending_period(sending_period),
    communication_thread_handle(nullptr) {}

CommunicationSubsystem::~CommunicationSubsystem()
{
    stopCommunication();
}

void CommunicationSubsystem::init()
{
    Ethernet *eth = Ethernet::getInstance();
    eth->init();
    mqtt.init(TOKEN, THINGSBOARD_SERVER);

    while (!eth->isConnected()) {
        ESP_LOGI("MAIN", "Waiting for Ethernet connection...");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


void CommunicationSubsystem::startCommunication()
{
    if (communication_thread_handle != nullptr)
    {
        vTaskDelete(communication_thread_handle);
        communication_thread_handle = nullptr;
    }
    xTaskCreate(
        CommunicationSubsystem::communicationRoutine,
        "CommunicationRoutine",
        4096,
        this,
        5,
        &communication_thread_handle
    );
}

void CommunicationSubsystem::stopCommunication()
{
    if (communication_thread_handle != nullptr)
    {
        vTaskDelete(communication_thread_handle);
        communication_thread_handle = nullptr;
    }
}

void CommunicationSubsystem::communicationRoutine(void *pvParameters)
{
    CommunicationSubsystem *communication_subsystem = (CommunicationSubsystem *)pvParameters;
    std::vector<ProcessedData> values;
    values.reserve(10);
    while (true)
    {   
        ProcessedData data = sending_queue.pop();
        values.push_back(data);
        if (values.size() == 10) {
            char payload[1024];
            std::ostringstream json;
            json << "{\"acceleration_x\":[";
            for (size_t i = 0; i < values.size(); ++i) {
                json << std::fixed << std::setprecision(2) << values[i].acceleration_x;
                if (i != values.size() - 1) {
                    json << ",";
                }
            }
            json << "],\"acceleration_y\":[";
            for (size_t i = 0; i < values.size(); ++i) {
                json << std::fixed << std::setprecision(2) << values[i].acceleration_y;
                if (i != values.size() - 1) {
                    json << ",";
                }
            }
            json << "],\"acceleration_z\":[";
            for (size_t i = 0; i < values.size(); ++i) {
                json << std::fixed << std::setprecision(2) << values[i].acceleration_z;
                if (i != values.size() - 1) {
                    json << ",";
                }
            }
            json << "]}";
            strncpy(payload, json.str().c_str(), sizeof(payload));
            payload[sizeof(payload) - 1] = '\0';
            values.clear();
            communication_subsystem->mqtt.sendJsonData(payload); 
            communication_subsystem->mqtt.loop();
        }
        communication_subsystem->mqtt.connect();
        vTaskDelay(communication_subsystem->sending_period / portTICK_PERIOD_MS);
    }
}