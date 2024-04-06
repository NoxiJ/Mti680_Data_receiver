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

CallbackHandler_Reader &MtiDataReader::getCallbackHandler() {
    return _callbackHandler;
}

/**
 * Initialisation du port du capteur
 * @param specificPort TRUE si on s'intéresse à un port specifique
 * @return TRUE si le port est bien initialise
 */
void MtiDataReader::init() {
    bool isSpecificPort = true;
    std::cout << "Scanning for devices..." << std::endl;
    XsPortInfoArray portInfoArray = XsScanner::scanPorts();

    // Find an MTi device
    for (auto const &portInfo: portInfoArray) {
        if (portInfo.deviceId().isMti() || portInfo.deviceId().isMtig()) {
            _mtPort = portInfo;
            std::cout << "Found a device with ID: " << _mtPort.deviceId().toString().toStdString()
                      << " @ port: " << _mtPort.portName().toStdString()
                      << ", baudrate: " << _mtPort.baudrate() << std::endl;
            isSpecificPort = false;
            break;
        }
    }
    if (isSpecificPort) {
        // Si scanPorts ne fonctionne pas, rentrer le port specifique du capteur
        _mtPort = initialize_specificPort(3);
    }
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
 * Fonctionnement :
 *      Lorsque le capteur passe en mode de configuration (goToConfig), il est alors possible d'activer les grandeurs
 *      que l'on souhaite mesurer en ajoutant des paramètres de configuration avec leur fréquence d'échantillonnage
 *      à un vecteur d'options.
 * @return FALSE si la configuration échoue
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

    if (_device->areOptionsEnabled(XsOption::XSO_Calibrate)) {
        std::cout << "calibrated inertial data from raw data and temperature ACTIVEE" << std::endl;
    } else {
        _device->setOptions(XSO_Calibrate, XSO_None);   // On active les calibrated data
    }

    // Put the device into configuration mode
    std::cout << "Putting device into configuration mode..." << std::endl;
    if (!_device->gotoConfig()) {
        std::cerr << "Failed to put device into configuration mode." << std::endl;
        return false;
    }

    // Configure the device
    /*
     * Pour cette partie, se referer au constructeur XsOutputConfiguration
     * Si le parametre de configuration n'a pas de frequence à configurer, entrer la valeur 0xFFFF
     */
    std::cout << "Configuring the device..." << std::endl;
    XsOutputConfigurationArray configArray;

    configArray.push_back(XsOutputConfiguration(XDI_PacketCounter, 0xFFFF));    // Compteur des paquets
    configArray.push_back(XsOutputConfiguration(XDI_SubFormatDouble, 0xFFFF));  // Format de mesure en double

    if (_device->deviceId().isGnss()) {
        configArray.push_back(XsOutputConfiguration(XDI_MagneticField, 100));   // Champ magnetique (100 Hz)
        configArray.push_back(XsOutputConfiguration(XDI_BaroPressure, 100));    // Pression atmospherique (100 Hz)
        configArray.push_back(XsOutputConfiguration(XDI_Acceleration, 200));    // Acceleration (200 Hz)
        configArray.push_back(XsOutputConfiguration(XDI_RateOfTurn, 200));      // Tx de rot    (200 Hz)

    }

    if (!_device->setOutputConfiguration(configArray)) {
        std::cerr << "Failed to configure MTi device." << std::endl;
        return false;
    }

    return true;
}

/**
 * Creation du fichier LOG :
 * Fonctionnement :
 *      Cree un fichier "logfile.mtb" a l'emplacement de l'execution
 * @return TRUE si la creation du fichier a reussi
 */
bool MtiDataReader::createLogFile() {
    std::cout << "Creating Log File ..." << std::endl;
    std::string fileName = "logfile.mtb";
    if (_device->createLogFile(fileName) != XRV_OK) {
        std::cerr << "Failed to create a log file. Aborting" << std::endl;
        return false;
    }

    return true;
}

/**
 * Demarrage de l'acquisition de donnees
 * Fonctionnement :
 *      Passe le capteur en mode mesure puis demarre l'aquisition des donnees
 * @return TRUE si l'aquisition a correctement demarre
 */
bool MtiDataReader::startRecording() {
    if (_device == nullptr) {
        std::cerr << "Device object is not initialized." << std::endl;
        return false;
    }

    // Put the device into measurement mode
    std::cout << "Putting device into measurement mode..." << std::endl;
    if (!_device->gotoMeasurement()) {
        std::cerr << "Failed to put device into measurement mode." << std::endl;
        return false;
    }

    // Start recording
    std::cout << "Starting recording..." << std::endl;
    return _device->startRecording();
}

/**
 * Arret de l'aquisition des donnees
 * Fonctionnement :
 *      Stop l'aquisition des donnees
 * @return TRUE si l'acquisition des donnees s'est bien arrete
 */
bool MtiDataReader::stopRecording() {
    if (_device == nullptr) {
        std::cerr << "Device object is not initialized." << std::endl;
        return false;
    }

    // Stop recording
    std::cout << "Stopping recording..." << std::endl;
    return _device->stopRecording();
}

/**
 * Fermeture du fichier LOG
 * Fonctionnement :
 *      Sauvegarde puis ferme le fichier de LOG
 * @return TRUE si la fermeture a reussi
 */
bool MtiDataReader::closeLogFile() {
    if (_device == nullptr) {
        std::cerr << "Device object is not initialized." << std::endl;
        return false;
    }

    // Close log file
    std::cout << "Closing log file..." << std::endl;
    return _device->closeLogFile();
}

/**
 * Fermeture du port precedemment ouvert
 */
void MtiDataReader::closePort() {
    if (!_mtPort.empty()) {
        std::cout << "Closing port..." << std::endl;
        _control->closePort(_mtPort.portName().toStdString());
    }
}

/**
 * Liberation de la memoire
 */
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

/**
 * Permet d'ouvrir un port specifique dans le cas ou scanPorts() ne retourne rien
 * @param portNumber numero du port a ouvrir
 * @return L'objet XsPortInfo contenant les donnees du port ouvert
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
