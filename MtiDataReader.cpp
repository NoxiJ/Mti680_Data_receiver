//
// Created by louis on 23/03/2024.
//

#include "MtiDataReader.h"

/*
 * METHODES PUBLIQUES
 */

/**
 * CONSTRUCTEUR : Initialze les instances de bases pour l'utilisation de l'API Xsens
 */
MtiDataReader::MtiDataReader() {
    _control = XsControl::construct();
    assert(control != nullptr);

    XsVersion version;
    xdaVersion(&version);
    std::cout << "Using XDA version: " << version.toString().toStdString() << std::endl;
}

/**
 * DESTRUCTEUR
 */
MtiDataReader::~MtiDataReader() {
    if (_control != nullptr) {
        _control->destruct();
        _control = nullptr;
    }
}

/**
 * GETTER *_control
 * @return *_control
 */
XsControl MtiDataReader::getControl() const {
    return *_control;
}

CallbackHandler &MtiDataReader::getCallbackHandler() {
    return _callbackHandler;
}

/**
 * Initialisation du port du capteur
 * @param specificPort TRUE si on s'intéresse à un port specifique
 * @return TRUE si le port est bien initialise
 */
bool MtiDataReader::initialize(bool specificPort) {
    if (!specificPort) {
        std::cout << "Scanning for devices..." << std::endl;
        XsPortInfoArray portInfoArray = XsScanner::scanPorts();

        // Find an MTi device
        for (auto const &portInfo: portInfoArray) {
            if (portInfo.deviceId().isMti() || portInfo.deviceId().isMtig()) {
                _mtPort = portInfo;
                std::cout << "Found a device with ID: " << _mtPort.deviceId().toString().toStdString()
                          << " @ port: " << _mtPort.portName().toStdString()
                          << ", baudrate: " << _mtPort.baudrate() << std::endl;
                return true;
            }
        }
    } else {
        // Par defaut on utlise le port 5 pour l'instant
        _mtPort = initialize_specificPort(5);
        return true;
    }

    std::cerr << "No MTi device found. Aborting." << std::endl;
    return false;
}

/**
 * Ouverture du port initialise
 * @return FALSE si l'ouverture echoue
 */
bool MtiDataReader::openPort() {
    if (_mtPort.empty()) {
        std::cerr << "MTi port is not initialized." << std::endl;
        return false;
    }

    std::cout << "Opening Port..." << std::endl;
    return _control->openPort(_mtPort.portName().toStdString(), _mtPort.baudrate());
}

/**
 * Configuration du capteur
 * @return FALSE si la configuration echoue
 */
bool MtiDataReader::configureDevice() {
    if (_control == nullptr || _mtPort.empty()) {
        std::cerr << "Control object or MTi port is not initialized." << std::endl;
        return false;
    }

    // Get the device object
    _device = _control->device(_mtPort.deviceId());
    if (_device == nullptr) {
        std::cerr << "Failed to get device object." << std::endl;
        return false;
    }

    // Create and attach callback handler to device
    _device->addCallbackHandler(&_callbackHandler);


    // Put the device into configuration mode
    std::cout << "Putting device into configuration mode..." << std::endl;
    if (!_device->gotoConfig()) {
        std::cerr << "Failed to put device into configuration mode." << std::endl;
        return false;
    }

    // Configure the device
    std::cout << "Configuring the device..." << std::endl;
    XsOutputConfigurationArray configArray;
    configArray.push_back(XsOutputConfiguration(XDI_PacketCounter, 0));
    configArray.push_back(XsOutputConfiguration(XDI_SampleTimeFine, 0));

    if (_device->deviceId().isImu()) {
        std::cout << "on passe dans isImu" << std::endl;
        configArray.push_back(XsOutputConfiguration(XDI_Acceleration, 100));
        configArray.push_back(XsOutputConfiguration(XDI_RateOfTurn, 100));
        configArray.push_back(XsOutputConfiguration(XDI_MagneticField, 100));
    } else if (_device->deviceId().isVru() || _device->deviceId().isAhrs()) {
        std::cout << "ON passe dans isVru ou isAhrs" << std::endl;
        configArray.push_back(XsOutputConfiguration(XDI_Quaternion, 100));
    } else if (_device->deviceId().isGnss()) {
        configArray.push_back(XsOutputConfiguration(XDI_Quaternion, 100));
        configArray.push_back(XsOutputConfiguration(XDI_LatLon, 100));
        configArray.push_back(XsOutputConfiguration(XDI_AltitudeEllipsoid, 100));
        configArray.push_back(XsOutputConfiguration(XDI_VelocityXYZ, 100));
    } else {
        std::cerr << "Unknown device while configuring. Aborting." << std::endl;
        return false;
    }

    if (!_device->setOutputConfiguration(configArray)) {
        std::cerr << "Failed to configure MTi device." << std::endl;
        return false;
    }



    // Put the device into measurement mode
    std::cout << "Putting device into measurement mode..." << std::endl;
    if (!_device->gotoMeasurement()) {
        std::cerr << "Failed to put device into measurement mode." << std::endl;
        return false;
    }

    return true;
}

bool MtiDataReader::startRecording() {
    if (_device == nullptr) {
        std::cerr << "Device object is not initialized." << std::endl;
        return false;
    }

    // Start recording
    std::cout << "Starting recording..." << std::endl;
    return _device->startRecording();
}

bool MtiDataReader::stopRecording() {
    if (_device == nullptr) {
        std::cerr << "Device object is not initialized." << std::endl;
        return false;
    }

    // Stop recording
    std::cout << "Stopping recording..." << std::endl;
    return _device->stopRecording();
}

bool MtiDataReader::closeLogFile() {
    if (_device == nullptr) {
        std::cerr << "Device object is not initialized." << std::endl;
        return false;
    }

    // Close log file
    std::cout << "Closing log file..." << std::endl;
    return _device->closeLogFile();
}

void MtiDataReader::closePort() {
    if (!_mtPort.empty()) {
        std::cout << "Closing port..." << std::endl;
        _control->closePort(_mtPort.portName().toStdString());
    }
}

void MtiDataReader::freeControlObject() {
    std::cout << "Freeing XsControl object..." << std::endl;
    if (_control != nullptr) {
        _control->destruct();
        _control = nullptr;
    }
}

/*
 * METHODES PRIVEES
 */

XsPortInfo MtiDataReader::initialize_specificPort(int portNumber) {
    XsPortInfo mtPort(portNumber);
    bool isPort = XsScanner::scanPort(mtPort);
    if (isPort) {
        std::cout << "Port connecte" << std::endl;
    } else {
        std::cout << "Port introuvable" << std::endl;
    }
    std::cout << "Found a device with ID: " << mtPort.deviceId().toString().toStdString()
              << " @ port: " << mtPort.portName().toStdString()
              << ", baudrate: " << mtPort.baudrate() << std::endl;

    return mtPort;
}
