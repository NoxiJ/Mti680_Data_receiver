//
// Created by louis on 24/03/2024.
//

#include <xsensdeviceapi.h>
#include <xstypes/xstime.h>
#include <xscommon/xsens_mutex.h>

#include <iostream>
#include <iomanip>
#include <csignal>
#include <list>
#include <string>
#include <conio.h>

#include "MtiDataReader.h"
#include "MtiParser.h"

volatile std::sig_atomic_t flag_interruption = 0;

int main(int argc, char* argv[]) {

    MtiDataReader dataReader;
    dataReader.init(TRUE);       // A tester d'abord dans le logiciel Xsens

    dataReader.openPort();
    dataReader.configureDevice();
    std::cout << "Configuration terminée" << std::endl;

    dataReader.createLogFile();

    std::cout << "Press [ENTER] to continue." << std::endl;
    std::cin.get();

    dataReader.startRecording();

    while (flag_interruption == 0) {

        /*
         * Partie pour s'assurer de la bonne réception des données
         */
        if (dataReader.getCallbackHandler().packetAvailable()) {
            std::cout << std::setw(5) << std::fixed << std::setprecision(2);

            // Retrieve a packet
            XsDataPacket packet = dataReader.getCallbackHandler().getNextPacket();
            std::cout << packet.packetCounter() << "\t";

            if (packet.containsCalibratedData()) {

                // ------------- ACCELERATION -------------- //
                XsVector acc = packet.calibratedAcceleration();
                std::cout << "\r"
                     << "Acc X:" << acc[0]
                     << ", Acc Y:" << acc[1]
                     << ", Acc Z:" << acc[2];

                // ------------- GYROSCOPE -------------- //
                XsVector gyr = packet.calibratedGyroscopeData();
                std::cout << " | Gyr X:" << gyr[0]
                          << ", Gyr Y:" << gyr[1]
                          << ", Gyr Z:" << gyr[2];

                // ------------- MAGNITUDE -------------- //
                XsVector mag = packet.calibratedMagneticField();
                std::cout << " | Mag X:" << mag[0]
                          << ", Mag Y:" << mag[1]
                          << ", Mag Z:" << mag[2];
            }

            if (packet.containsPressure()) {
                XsPressure pressure = packet.pressure();
                std::cout << " | Pressure :" << pressure.m_pressure;
            }
            std::cout<<std::endl;
        }

        // --------------------------------------------------------------- //

        if (_kbhit()) {
            // Si la touche est "Entrée", sortir de la boucle
            if (_getch() == '\r') {
                flag_interruption = 1;
            }
        }

        XsTime::msleep(0); // TEMPO
    }

    // ----- LIBERATION MEMOIRE ------ //
    dataReader.stopRecording();
    dataReader.closeLogFile();
    dataReader.closePort();
    dataReader.freeControlObject();
    // ------------------------------- //

    std::cout << "Successful exit." << std::endl;
    std::cout << "Press [ENTER] to continue." << std::endl;
    std::cin.get();

    MtiParser parser;
    if (!parser.openLogFile()) {
        return 0;
    }

    parser.createDeviceInstance();
    parser.loadLogFile();
    parser.exportData();

    MtiDataValues values = parser.getValues();
    values.createExcelOutputFile("test.xlsx");
    values.writeHeaders();
    values.writeData();

    values.closeWorkbook();

    return 0;
}
