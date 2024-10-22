#include "AwaitingMonitoring.hpp"
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "Monitoring.hpp"
#include "VibrationSystem.hpp"

void AwaitingMonitoring::Enter() {
  ESP_LOGI("FSM", "Awating Signal to Start Monitoring");
  start_time = esp_timer_get_time();
  _vibration_system->startActiveDetection();
}

void AwaitingMonitoring::Update() {
  if (_vibration_system->getIsActive()) {
    _vibration_system->trasitionTo(new Monitoring());
    return;
  }

  int64_t elapsed_time = esp_timer_get_time() - start_time;
  if (elapsed_time > 1000000 and !_vibration_system->checkActiveDetectionTimer()) { 
      _vibration_system->stopActiveDetection();
      _vibration_system->startActiveDetection();
  }
}

void AwaitingMonitoring::Exit() {
  ESP_LOGI("FSM", "Start Monitoring");
  _vibration_system->stopActiveDetection();
}