//
// Created by louis on 23/03/2024.
//

#include "MtiDataValues.h"

/**
 * CONSTRUCTEUR
 */
MtiDataValues::MtiDataValues() {

}

/**
 * DESCTRUCTEUR
 */
MtiDataValues::~MtiDataValues() {

}

void MtiDataValues::addPacketCounters(const uint16_t counter) {
    _packetCounters.push_back(counter);
}

void MtiDataValues::addAcceleration(const XsVector& acceleration) {
    _accelerations.push_back(acceleration);
}

void MtiDataValues::addGyroscope(const XsVector& gyroscope) {
    _gyroscopes.push_back(gyroscope);
}

void MtiDataValues::addMagnitude(const XsVector& magnitude) {
    _magnitudes.push_back(magnitude);
}

void MtiDataValues::addPressure(const XsPressure& baroPressure) {
    _baroPressure.push_back(baroPressure);
}

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

std::vector<XsVector> MtiDataValues::getAccelerations() {
    return _accelerations;
}

std::vector<XsVector> MtiDataValues::getGyroscopes() {
    return _gyroscopes;
}

std::vector<XsVector> MtiDataValues::getMagnitudes() {
    return _magnitudes;
}

std::vector<XsPressure> MtiDataValues::getBaroPressure() {
    return _baroPressure;
}

void MtiDataValues::createExcelOutputFile(const char *fileName) {
    _workbook = workbook_new(fileName);
    _worksheet = workbook_add_worksheet(_workbook, "raw_data");
}

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

    for (int i = 0; i < 11; i++) {
        worksheet_write_string(_worksheet, 0, i, headerNames[i], nullptr);
    }
}

void MtiDataValues::writeData() {
    std::cout << _packetCounters.size() << std::endl;
    std::cout << _accelerations.size() << std::endl;
    for (int i = 0; i < _packetCounters.size(); i++) {
        worksheet_write_number(_worksheet, i + 1, 0, _packetCounters[i], nullptr);
//        std::cout << _accelerations[i][0] << std::endl;
//        worksheet_write_number(_worksheet, i + 1, 1, _accelerations[i][0], nullptr);
    }
}

void MtiDataValues::closeWorkbook() {
    std::cout << "fermeture" << std::endl;
    workbook_close(_workbook);
}