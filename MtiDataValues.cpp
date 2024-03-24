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

void MtiDataValues::addAcceleration(XsVector acceleration) {
    _accelerations.push_back(acceleration);
}

void MtiDataValues::addGyroscope(XsVector gyroscope) {
    _gyroscopes.push_back(gyroscope);
}

void MtiDataValues::addMagnitude(XsVector magnitude) {
    _magnitudes.push_back(magnitude);
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