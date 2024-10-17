set(FILE_ACQUISTION_SRCS
    <AcquisitionSubsystem/ADXL345/components/Adafruit_ADXL345/Adafruit_ADXL345_U.cpp>
    <AcquisitionSubsystem/ADXL345/components/Adafruit_Sensor/Adafruit_Sensor.cpp>
    <AcquisitionSubsystem/ADXL345/components/Adafruit_BusIO/Adafruit_I2CDevice.cpp>
    <AcquisitionSubsystem/ADXL345/components/Adafruit_BusIO/Adafruit_SPIDevice.cpp>
    <AcquisitionSubsystem/ADXL345/ADXL345.cpp>
    <AcquisitionSubsystem/AcquisitionSubsystem.cpp>
)

set(FILE_COMUNICATION_SRCS
    <ComunicationSubsystem/Ethernet/Ethernet.cpp>
    <ComunicationSubsystem/MQTT/MQTT.cpp>
)

idf_component_register(
    SRCS 
        "main.cpp"
        ${FILE_ACQUISTION_SRCS}
        ${FILE_COMUNICATION_SRCS}
        "BlockingQueue/BlockingQueue.cpp"

    INCLUDE_DIRS 
        "."

        "AcquisitionSubsystem/ADXL345/components/Adafruit_ADXL345/"
        "AcquisitionSubsystem/ADXL345/components/Adafruit_Sensor/"
        "AcquisitionSubsystem/ADXL345/components/Adafruit_BusIO/"
        "AcquisitionSubsystem/ADXL345/"
        "AcquisitionSubsystem/"

        "ComunicationSubsystem/MQTT/"
        "ComunicationSubsystem/Ethernet/"
        
        "BlockingQueue/"
)
