#ifndef ACQUISITION_SUBSYSTEM_HPP
#define ACQUISITION_SUBSYSTEM_HPP

#include "ADXL345.hpp"
#include <thread>

class AcquisitionSubsystem {
public:
    AcquisitionSubsystem(int sampling_period = 2, int mean_window_size = 50);

    void init();
    void startAcquisition();
    void stopAcquisition();
    void AcquisitionRoutine();

    bool getIsMoving();

private:
    ADXL345 accelerometer;  // m/s^2
    std::thread acquisition_thread;
    std::atomic<bool> is_running;
    int sampling_period; // ms
    int mean_window_size;   
};

#endif // ACQUISITION_SUBSYSTEM_HPP