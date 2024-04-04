//
// Created by louis on 04/04/2024.
//

#include "MtiParser.h"

MtiParser::MtiParser() {
    _control = XsControl::construct();
    assert(_control != nullptr);
}

MtiParser::~MtiParser() {
    if (_control != nullptr) {
        _control->destruct();
        _control = nullptr;
    }
}

/*
 *  GETTERS
 */
XsControl MtiParser::getControl() const {
    return *_control;
}

CallbackHandler &MtiParser::getCallbackHandler() {
    return _callbackHandler;
}

/*
 * METHODES PUBLIQUES
 */
bool MtiParser::openLogFile() {
    std::cout << "Opening log file..." << std::endl;
    std::string logFileName = "logfile.mtb";
    if (!_control->openLogFile(logFileName)) {
        std::cerr << "Failed to open log file" << std::endl;
        return false;
    }
    return true;
}

void MtiParser::createDeviceInstance() {
    XsDeviceIdArray deviceIdArray = _control->mainDeviceIds();
    XsDeviceId mtDevice;
    // On cherche le device dans le doc
    for (auto const &deviceId : deviceIdArray) {
        if (deviceId.isMti() || deviceId.isMtig()) {
            mtDevice = deviceId;
            break;
        }
    }
    if (!mtDevice.isValid()) {
        std::cerr << "No Mti device found" << std::endl;
        return;
    }
    _device = _control->device(mtDevice);
    assert(device != nullptr);
    std::cout << "Device: " << _device->productCode() << ", with ID: " << _device->deviceId().toString()
        << " found in file." << std::endl;
}

void MtiParser::loadLogFile() {
    // On ajoute le callBack handler
    _device->addCallbackHandler(&_callbackHandler);
    /*
     * Cette option permet de garder les donneees dans un buffer de data
     * qui peut alors être acquerie par XsDevice::getDataPacketByIndex ou XsDevice::takeFirstDataPacketInQueue
     */
    _device->setOptions(XSO_RetainBufferedData, XSO_None);

    // Chargement du fichier Log
    std::cout << "Loading the file ..." << std::endl;
    _device->loadLogFile();
    while(_callbackHandler.progress() != 100)
        xsYield();
    std::cout << "File is fully loaded ..." << std::endl;
}

void MtiParser::exportData() {
    // On recupere le nombre total de samples
    XsSize packetCount = _device->getDataPacketCount();
    for (XsSize i = 0; i < packetCount; i++) {

        XsDataPacket packet = _device->getDataPacketByIndex(i);

        if (packet.containsCalibratedData()) {

            // ------------- ACCELERATION -------------- //
            XsVector acc = packet.calibratedAcceleration();
            std::cout << "\r"
                      << "Acc X:" << acc[0]
                      << ", Acc Y:" << acc[1]
                      << ", Acc Z:" << acc[2];
            _dataValues.addAcceleration(acc);    // stockage des donnees

            // ------------- GYROSCOPE -------------- //
            XsVector gyr = packet.calibratedGyroscopeData();
            std::cout << " | Gyr X:" << gyr[0]
                      << ", Gyr Y:" << gyr[1]
                      << ", Gyr Z:" << gyr[2];
            _dataValues.addGyroscope(gyr);       // stockage des donnees

            // ------------- MAGNITUDE -------------- //
            XsVector mag = packet.calibratedMagneticField();
            std::cout << " | Mag X:" << mag[0]
                      << ", Mag Y:" << mag[1]
                      << ", Mag Z:" << mag[2];
            _dataValues.addMagnitude(mag);       // stockage des donnees
        }

        if (packet.containsPressure()) {
            XsPressure pressure = packet.pressure();
            std::cout << " | Pressure :" << pressure.m_pressure;
            _dataValues.addPressure(pressure);
        }
        std::cout<<std::endl;
    }
}
