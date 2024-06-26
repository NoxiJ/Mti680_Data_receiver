//
// Created by louis on 23/03/2024.
//

#include "MtiDataValues.h"

/**
 * CONSTRUCTEUR
 */
MtiDataValues::MtiDataValues() {}

/**
 * DESCTRUCTEUR
 */
MtiDataValues::~MtiDataValues() {}

/**
 * Ajoute une valeur a la liste de packet counter
 * @param counter
 */
void MtiDataValues::addPacketCounters(uint16_t counter) {
    _packetCounters.push_back(counter);
}

/**
 * Ajoute des composantes d'acceleration a la liste
 * @param acceleration
 */
void MtiDataValues::addAcceleration(const XsVector& acceleration) {
    _accelerations.push_back(acceleration);
}

/**
 * Ajoute des composantes de taux de rotation a la liste
 * @param gyroscope
 */
void MtiDataValues::addGyroscope(const XsVector& gyroscope) {
    _gyroscopes.push_back(gyroscope);
}

/**
 * Ajoute des composantes de champ magnetique a la liste
 * @param magnitude
 */
void MtiDataValues::addMagnitude(const XsVector& magnitude) {
    _magnitudes.push_back(magnitude);
}

/**
 * Ajoute une valeur de pression a la liste
 * @param baroPressure
 */
void MtiDataValues::addPressure(const double& baroPressure) {
    _baroPressure.push_back(baroPressure);
}

/*
 * CLEARERS
 */
void MtiDataValues::clearAccelerations() {
    _accelerations.clear();
}

void MtiDataValues::clearGyroscopes() {
    _gyroscopes.clear();
}

void MtiDataValues::clearMagnitudes() {
    _magnitudes.clear();
}

void MtiDataValues::clearBaroPressure() {
    _baroPressure.clear();
}

/*
 * GETTERS
 */
std::vector<XsVector> MtiDataValues::getAccelerations() {
    return _accelerations;
}

std::vector<XsVector> MtiDataValues::getGyroscopes() {
    return _gyroscopes;
}

std::vector<XsVector> MtiDataValues::getMagnitudes() {
    return _magnitudes;
}

std::vector<double> MtiDataValues::getBaroPressure() {
    return _baroPressure;
}

/**
 * Creation d'un fichier de sortie Excel
 * @param fileName nom du fichier de sortie
 */
void MtiDataValues::createExcelOutputFile(const char *fileName) {
    _workbook = workbook_new(fileName);
    _worksheet = workbook_add_worksheet(_workbook, "raw_data");
}

/**
 * Ecriture des titres des donnees sur la spreasheet Excel
 */
void MtiDataValues::writeHeaders() {
    static const char* const headerNames[] = {"Packet Counter",
                                              "Acc_X",
                                              "Acc_Y",
                                              "Acc_Z",
                                              "Gyro_X",
                                              "Gyro_Y",
                                              "Gyro_Z",
                                              "Mag_X",
                                              "Mag_Y",
                                              "Mag_Z",
                                              "Pressure"};

    for (int i = 0; i <= 10; i++) {
        worksheet_write_string(_worksheet, 0, i, headerNames[i], nullptr);
    }
}

/**
 * Ecriture des donnees dans la spreadsheet Excel
 */
void MtiDataValues::writeData() {
    for (int i = 0; i < _accelerations.size(); i++) {
        worksheet_write_number(_worksheet, i + 1, 0, _packetCounters[i], nullptr);
        worksheet_write_number(_worksheet, i + 1, 1, _accelerations[i][0], nullptr);
        worksheet_write_number(_worksheet, i + 1, 2, _accelerations[i][1], nullptr);
        worksheet_write_number(_worksheet, i + 1, 3, _accelerations[i][2], nullptr);
        worksheet_write_number(_worksheet, i + 1, 4, _gyroscopes[i][0], nullptr);
        worksheet_write_number(_worksheet, i + 1, 5, _gyroscopes[i][1], nullptr);
        worksheet_write_number(_worksheet, i + 1, 6, _gyroscopes[i][2], nullptr);
    }

    for (int i = 0; i < _baroPressure.size(); i++) {
        worksheet_write_number(_worksheet, (i + 1) * 2, 7, _magnitudes[i][0], nullptr);
        worksheet_write_number(_worksheet, (i + 1) * 2, 8, _magnitudes[i][1], nullptr);
        worksheet_write_number(_worksheet, (i + 1) * 2, 9, _magnitudes[i][2], nullptr);
        worksheet_write_number(_worksheet, (i + 1) * 2, 10, _baroPressure[i], nullptr);
    }
}

/**
 * Fermeture du fichier Excel
 */
void MtiDataValues::closeWorkbook() {
    std::cout << "fermeture" << std::endl;
    workbook_close(_workbook);
}