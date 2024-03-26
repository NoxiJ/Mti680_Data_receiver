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

    void addAcceleration    (XsVector acceleration);
    void addGyroscope       (XsVector gyroscope);
    void addMagnitude       (XsVector magnitude);

    void clearAccelerations ();
    void clearGyroscopes    ();
    void clearMagnitudes    ();

    std::vector<XsVector> getAccelerations();
    std::vector<XsVector> getGyroscopes();
    std::vector<XsVector> getMagnitudes();


private:

    std::vector<XsVector> _accelerations;
    std::vector<XsVector> _gyroscopes;
    std::vector<XsVector> _magnitudes;
};


#endif //MTI680_DATA_RECEIVER_MTIDATAVALUES_H
