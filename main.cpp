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

/**
 * Procédure principale :
 *      1ere Partie :   Instancie les objets nécessaires à l'acquisition de données du capteur
 *                      Lance l'acquisition lorsque l'utilisateur presse [ENTREE]
 *                      Stop l'acquisition lorsque l'utilisateur presse à nouveau [ENTREE]
 *                      Sauvegarde de l'acquisition dans un fichier Log au format .mtb
 *      2e Partie :     Instanciation des objets pour parse le fichier Log
 *                      Parse du fichier de Log
 *                      Création d'un fichier de sortie Excel
 *                      Écriture des données dans ce fichier
 *                      Fermeture/Sauvegarde du fichier de sortie
 */
int main(int argc, char* argv[]) {

    MtiDataReader dataReader;
    dataReader.init();

    dataReader.openPort();                  // Ouverture du port
    dataReader.configureDevice();           // Configuration de l'objet device
    std::cout << "Configuration terminée" << std::endl;

    dataReader.createLogFile();             // Création du fichier de Log

    std::cout << "Press [ENTER] to continue." << std::endl;
    std::cin.get();

    dataReader.startRecording();
    std::cout << "Press [ENTER] to stop recording." << std::endl;

    while (flag_interruption == 0) {

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

    // ------ PARSE DU FICHIER DE LOG -------- //
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
