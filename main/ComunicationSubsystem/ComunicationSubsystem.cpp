#include "CommunicationSubsystem.hpp"

CommunicationSubsystem::CommunicationSubsystem()
    :is_running(false)
{
}

CommunicationSubsystem::~CommunicationSubsystem()
{
    stopCommunication();
}

void CommunicationSubsystem::initi()
{
    Ethernet *eth = Ethernet::getInstance();
}