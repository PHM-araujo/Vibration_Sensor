#ifndef MQTT_HPP
#define MQTT_HPP

#include <ThingsBoard.h>
#include <Espressif_MQTT_Client.h>
#include <string.h>

class MQTT
{ 
public:
    MQTT(int8_t reconnect_count = 10, uint16_t MAX_MESSAGE_SIZE = 128);
    ~MQTT();
    void init(std::string token, std::string server);
    bool connect();
    bool sendData(std::string key, float value);
    void loop() { tb->loop(); }

private:
    ThingsBoard *tb;
    Espressif_MQTT_Client<DefaultLogger> mqttClient;
    int8_t reconnect_count;
    std::string token;
    std::string thingsboard_server;
};

#endif // MQTT_HPP