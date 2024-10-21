#ifndef VIBRATIONSYSTEM_HPP
#define VIBRATIONSYSTEM_HPP

#include "AcquisitionSubsystem.hpp"
#include "CommunicationSubsystem.hpp"
#include "ProcessingSubsystem.hpp"
#include "State.hpp"

class VibrationSystem {
public:
    VibrationSystem();
    ~VibrationSystem();

    void trasitionTo(State *new_state);
    void init(State *initial_state);
    void initSubsystems();

    bool getIsActive();
    void startActiveDetection();
    void stopActiveDetection();
    void startInactiveDetection();
    void stopInactiveDetection();
    void startMonitoringRoutines();
    void stopMonitoringRoutines();

private:
    static void updateRoutine(void *pvParameters);

    static State *state;
    AcquisitionSubsystem acquisition_subsystem;
    CommunicationSubsystem communication_subsystem;
    ProcessingSubsystem processing_subsystem;
};

#endif // VIBRATIONSYSTEM_HPP