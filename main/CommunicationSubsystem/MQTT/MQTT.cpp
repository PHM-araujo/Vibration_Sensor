#include "MQTT.hpp"
#include <esp_log.h>


MQTT::MQTT(int8_t reconnect_count, uint16_t max_message_size)
    :reconnect_count(reconnect_count), token(""), thingsboard_server("")
{
    tb = new ThingsBoard(mqttClient, max_message_size);
}

MQTT::~MQTT()
{
    delete tb;
}

void MQTT::init(std::string token, std::string server)
{
    this->token = token;
    this->thingsboard_server = server;
}

bool MQTT::connect()
{
    int8_t count = 0;
    while (!tb->connected() and count < this->reconnect_count) {
        tb->connect(this->thingsboard_server.c_str(), this->token.c_str());
        ESP_LOGI("MAIN", "Trying to connect to the server...");
        count++;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    if (count == this->reconnect_count) {
        ESP_LOGI("MAIN", "Failed to connect to the server!");
        return false;
    } else {
        ESP_LOGI("MAIN", "Connected to the server!");
        return true;
    }
}

bool MQTT::sendData(std::string key, float value)
{
    bool answ = tb->sendTelemetryData(key.c_str(), value);
    if (!answ) {
        ESP_LOGI("MAIN", "Failed to send data");
        return false;
    } 
    ESP_LOGI("MAIN", "Data with key %s and value %f sent", key.c_str(), value);
    return true;
}

bool MQTT::sendJsonData(std::string json)
{
    bool answ = tb->sendTelemtryString(json.c_str());
    if (!answ) {
        ESP_LOGI("MAIN", "Failed to send data");
        return false;
    } 
    ESP_LOGI("MAIN", "Data with json %s sent", json.c_str());
    return true;
}
