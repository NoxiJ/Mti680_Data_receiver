/**
 * Classe MtiDataValues
 * @author Louis Jeauffroy
 * @date 23/03/2024
 * Description :
 *      Classe contenant les vecteurs liste contenant les données récupérées dans le fichier LOG .mtb.
 *      Elle permet également l'extraction de celles-ci vers une feuille d'un fichier Excel au format .xlsx ceci
 *      via l'utilisation de la bibliothèque C "xlsxWriter".
 */

#ifndef MTI680_DATA_RECEIVER_MTIDATAVALUES_H
#define MTI680_DATA_RECEIVER_MTIDATAVALUES_H

#include <xstypes.h>
#include <xlsxwriter.h>

#include <iostream>

class MtiDataValues {
public:

    MtiDataValues();
    ~MtiDataValues();

    void addPacketCounters  (uint16_t counter);
    void addAcceleration    (const XsVector& acceleration);
    void addGyroscope       (const XsVector& gyroscope);
    void addMagnitude       (const XsVector& magnitude);
    void addPressure        (const double& baroPressure);

    void clearAccelerations ();
    void clearGyroscopes    ();
    void clearMagnitudes    ();
    void clearBaroPressure  ();

    std::vector<XsVector>   getAccelerations();
    std::vector<XsVector>   getGyroscopes();
    std::vector<XsVector>   getMagnitudes();
    std::vector<double> getBaroPressure();

    void createExcelOutputFile  (const char *fileName);
    void writeHeaders           ();
    void writeData              ();
    void closeWorkbook          ();

private:

    std::vector<uint16_t>   _packetCounters;    // 200 Hz
    std::vector<XsVector>   _accelerations;     // 200 Hz
    std::vector<XsVector>   _gyroscopes;        // 200 Hz
    std::vector<XsVector>   _magnitudes;        // 100 Hz
    std::vector<double> _baroPressure;      // 100 Hz

    lxw_workbook *_workbook;
    lxw_worksheet *_worksheet;

};


#endif //MTI680_DATA_RECEIVER_MTIDATAVALUES_H
