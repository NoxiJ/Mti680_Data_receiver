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