set(FILE_ACQUISTION_SRCS
    <AcquisitionSubsystem/ADXL345/components/Adafruit_ADXL345/Adafruit_ADXL345_U.cpp>
    <AcquisitionSubsystem/ADXL345/components/Adafruit_Sensor/Adafruit_Sensor.cpp>
    <AcquisitionSubsystem/ADXL345/components/Adafruit_BusIO/Adafruit_I2CDevice.cpp>
    <AcquisitionSubsystem/ADXL345/components/Adafruit_BusIO/Adafruit_SPIDevice.cpp>
    <AcquisitionSubsystem/ADXL345/ADXL345.cpp>
    <AcquisitionSubsystem/AcquisitionSubsystem.cpp>
)

set(FILE_COMMUNICATION_SRCS
    <CommunicationSubsystem/Ethernet/Ethernet.cpp>
    <CommunicationSubsystem/MQTT/MQTT.cpp>
    <CommunicationSubsystem/CommunicationSubsystem.cpp>
)

set(FILE_PROCESSING_SRCS
    <ProcessingSubsystem/ProcessingSubsystem.cpp>
    <ProcessingSubsystem/LowPassFilter/LowPassFilter.cpp>
)

set(FILE_FSM_SRCS
    <FSM/AwaitingMonitoring/AwaitingMonitoring.cpp>
    <FSM/Monitoring/Monitoring.cpp>
    <FSM/StartingDevice/StartingDevice.cpp>
)


idf_component_register(
    SRCS 
        "main.cpp"
        ${FILE_ACQUISTION_SRCS}
        ${FILE_PROCESSING_SRCS}
        ${FILE_COMMUNICATION_SRCS}
        ${FILE_FSM_SRCS}
        "BlockingQueue/BlockingQueue.cpp"
        "VibrationSystem/VibrationSystem.cpp"

    INCLUDE_DIRS 
        "."

        "AcquisitionSubsystem/ADXL345/components/Adafruit_ADXL345/"
        "AcquisitionSubsystem/ADXL345/components/Adafruit_Sensor/"
        "AcquisitionSubsystem/ADXL345/components/Adafruit_BusIO/"
        "AcquisitionSubsystem/ADXL345/"
        "AcquisitionSubsystem/"

        "ProcessingSubsystem/"
        "ProcessingSubsystem/LowPassFilter/"

        "CommunicationSubsystem/MQTT/"
        "CommunicationSubsystem/Ethernet/"
        "CommunicationSubsystem/"

        "FSM/"
        "FSM/AwaitingMonitoring/"
        "FSM/Monitoring/"
        "FSM/StartingDevice/"
        
        
        "BlockingQueue/"

        "VibrationSystem/"

)
