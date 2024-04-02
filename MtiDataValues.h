//
// Created by louis on 23/03/2024.
//

#ifndef MTI680_DATA_RECEIVER_MTIDATAVALUES_H
#define MTI680_DATA_RECEIVER_MTIDATAVALUES_H

#include <xstypes.h>

class MtiDataValues {
public:

    MtiDataValues();
    ~MtiDataValues();

    void addAcceleration    (const XsVector& acceleration);
    void addGyroscope       (const XsVector& gyroscope);
    void addMagnitude       (const XsVector& magnitude);
    void addPressure        (const XsPressure& baroPressure);

    void clearAccelerations ();
    void clearGyroscopes    ();
    void clearMagnitudes    ();
    void clearBaroPressure  ();

    std::vector<XsVector>   getAccelerations();
    std::vector<XsVector>   getGyroscopes();
    std::vector<XsVector>   getMagnitudes();
    std::vector<XsPressure> getBaroPressure();

private:

    std::vector<uint16_t>   _packetCounters;    // 200 Hz
    std::vector<XsVector>   _accelerations;     // 200 Hz
    std::vector<XsVector>   _gyroscopes;        // 200 Hz
    std::vector<XsVector>   _magnitudes;        // 100 Hz
    std::vector<XsPressure> _baroPressure;      // 100 Hz

};


#endif //MTI680_DATA_RECEIVER_MTIDATAVALUES_H
