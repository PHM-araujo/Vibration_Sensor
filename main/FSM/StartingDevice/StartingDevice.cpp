#include "StartingDevice.hpp"
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "AwaitingMonitoring.hpp"
#include "VibrationSystem.hpp"

void StartingDevice::Enter() {
  ESP_LOGI("FSM", "StartingDevice::Enter");
  _vibration_system->initSubsystems();
}

void StartingDevice::Update() {
  ESP_LOGI("FSM", "StartingDevice::Update");
  _vibration_system->trasitionTo(new AwaitingMonitoring());
  return;
}

void StartingDevice::Exit() {
  ESP_LOGI("FSM", "StartingDevice::Exit");
}