#ifndef COMMUNICATION_SUBSYSTEM_HPP
#define COMMUNICATION_SUBSYSTEM_HPP

#include "Ethernet.hpp"
#include "MQTT.hpp"
#include <thread>

class CommunicationSubsystem {
public:
    CommunicationSubsystem();
    ~CommunicationSubsystem();

    void initi();
    void startCommunication();
    void stopCommunication();
    void communicationRoutine();

private:
    std::thread communication_thread;
    std::atomic<bool> is_running;
    MQTT mqtt;
};

#endif // COMMUNICATION_SUBSYSTEM_HPP