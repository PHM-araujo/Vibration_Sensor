#include "Monitoring.hpp"
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "VibrationSystem.hpp"
#include "AwaitingMonitoring.hpp"

void Monitoring::Enter() {
  ESP_LOGI("FSM", "Initializing Monitoring");
  _vibration_system->startMonitoringRoutines();
  _vibration_system->startInactiveDetection();
}

void Monitoring::Update() {
  if (!_vibration_system->getIsActive()) {
    _vibration_system->trasitionTo(new AwaitingMonitoring());
  }
}

void Monitoring::Exit() {
  ESP_LOGI("FSM", "Stopping Monitoring");
  _vibration_system->stopMonitoringRoutines();
  _vibration_system->stopInactiveDetection();
}