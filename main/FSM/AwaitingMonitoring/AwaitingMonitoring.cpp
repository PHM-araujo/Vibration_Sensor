#include "AwaitingMonitoring.hpp"
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "Monitoring.hpp"
#include "VibrationSystem.hpp"

void AwaitingMonitoring::Enter() {
  ESP_LOGI("FSM", "Awating Signal to Start Monitoring");
  _vibration_system->startActiveDetection();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  _vibration_system->startActiveDetection();
}

void AwaitingMonitoring::Update() {
  if (_vibration_system->getIsActive()) {
    _vibration_system->trasitionTo(new Monitoring());
  }
}

void AwaitingMonitoring::Exit() {
  ESP_LOGI("FSM", "Start Monitoring");
  _vibration_system->stopActiveDetection();
}