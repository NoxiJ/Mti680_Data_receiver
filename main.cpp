//
// Created by louis on 24/03/2024.
//

#include <xsensdeviceapi.h>
#include <xstypes/xstime.h>
#include <xscommon/xsens_mutex.h>

#include <iostream>
#include <iomanip>
#include <list>
#include <string>

#include "MtiDataReader.h"
#include "MtiDataValues.h"
#include "Excel_exporter.h"

#define DURATION 2000  // en millisecondes

int main() {

    MtiDataReader dataReader;
    MtiDataValues values;

    dataReader.initialize(FALSE);       // A tester d'abord dans le logiciel Xsens

    dataReader.openPort();
    dataReader.configureDevice();

    dataReader.startRecording();

    int64_t startTime = XsTime::timeStampNow();
    while (XsTime::timeStampNow() - startTime <= DURATION) {
        if (dataReader.getCallbackHandler().packetAvailable()) {
            std::cout << std::setw(5) << std::fixed << std::setprecision(2);

            // Retrieve a packet
            XsDataPacket packet = dataReader.getCallbackHandler().getNextPacket();

            if (packet.containsCalibratedData()) {

                std::cout << "ON RENTRE DANS LE IF(CONTAINS CALIBRATED DATA) !!!" << std::endl;

                // ------------- ACCELERATION -------------- //
                XsVector acc = packet.calibratedAcceleration();
                std::cout << "\r"
                     << "Acc X:" << acc[0]
                     << ", Acc Y:" << acc[1]
                     << ", Acc Z:" << acc[2];
                values.addAcceleration(acc);    // stockage des donnees

                // ------------- GYROSCOPE -------------- //
                XsVector gyr = packet.calibratedGyroscopeData();
                std::cout << " |Gyr X:" << gyr[0]
                          << ", Gyr Y:" << gyr[1]
                          << ", Gyr Z:" << gyr[2];
                values.addGyroscope(gyr);       // stockage des donnees

                // ------------- MAGNITUDE -------------- //
                XsVector mag = packet.calibratedMagneticField();
                std::cout << " |Mag X:" << mag[0]
                          << ", Mag Y:" << mag[1]
                          << ", Mag Z:" << mag[2];
                values.addMagnitude(mag);       // stockage des donnees
            }
        }

        XsTime::msleep(0); // TEMPO
    }

    // LIBERATION MEMOIRE
    dataReader.stopRecording();
    dataReader.closePort();
    dataReader.freeControlObject();

    std::cout << "Successful exit." << std::endl;
    std::cout << "Press [ENTER] to continue." << std::endl;
    std::cin.get();



    return 0;
}
