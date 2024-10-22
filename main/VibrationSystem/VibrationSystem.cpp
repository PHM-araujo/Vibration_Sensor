#include "VibrationSystem.hpp"

State *VibrationSystem::state = nullptr;

VibrationSystem::VibrationSystem() 
    : acquisition_subsystem(),
    communication_subsystem(),
    processing_subsystem() {}

VibrationSystem::~VibrationSystem() {
    delete state;
}

void VibrationSystem::trasitionTo(State *new_state) {
  ESP_LOGI("FSM", "TrasitionTo new state");
  // Verifica se o estado atual é diferente do novo estado
  if(state != new_state) {
    // Verifica se o estado atual não é nulo
    if(state != nullptr) {
      // Executa a função de saída do estado atual
      state->Exit();
      delete state;
    }

    // Altera o estado atual
    this->state = new_state;
    this->state->SetVibrationSystem(this); 

    // Verifica se o novo estado não é nulo
    if(state != nullptr) {
      // Executa a função de entrada do novo estado
      state->Enter();
    }
  }
}

void VibrationSystem::updateRoutine(void *pvParameters) {
  VibrationSystem *vibration_system = (VibrationSystem *)pvParameters;
  while (true) {
    vibration_system->state->Update();
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void VibrationSystem::init(State *initial_state) {
  trasitionTo(initial_state);
  xTaskCreate(&updateRoutine, "updateRoutine", 2048, this, 5, NULL);
}

void VibrationSystem::initSubsystems() {
  // Initialize subsystems with their respective execution periods
  acquisition_subsystem.init(2);
  processing_subsystem.init(2);
  communication_subsystem.init(20);
}

bool VibrationSystem::getIsActive() {
  return acquisition_subsystem.getIsMoving();
}

void VibrationSystem::startMonitoringRoutines() {
  acquisition_subsystem.startAcquisition();
  processing_subsystem.startProcessing();
  communication_subsystem.startCommunication();
}

void VibrationSystem::stopMonitoringRoutines() {
  acquisition_subsystem.stopAcquisition();
  processing_subsystem.stopProcessing();
  communication_subsystem.stopCommunication();
}

void VibrationSystem::startActiveDetection() {
  acquisition_subsystem.enableActivityDetection();
}

void VibrationSystem::stopActiveDetection() {
  acquisition_subsystem.disableActivityDetection();
}

void VibrationSystem::startInactiveDetection() {
  acquisition_subsystem.enableInactivityDetection();
}

void VibrationSystem::stopInactiveDetection() {
  acquisition_subsystem.disableInactivityDetection();
}

bool VibrationSystem::checkActiveDetectionTimer() {
  return acquisition_subsystem.getTimerIsAlive();
}

