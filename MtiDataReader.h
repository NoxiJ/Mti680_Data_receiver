//
// Created by louis on 23/03/2024.
//

#ifndef MTI680_DATA_RECEIVER_MTIDATAREADER_H
#define MTI680_DATA_RECEIVER_MTIDATAREADER_H

#include <xsensdeviceapi.h>
#include <xstypes/xstime.h>
#include <xscommon/xsens_mutex.h>
#include <iostream>
#include <iomanip>
#include <list>
#include <string>

class MtiDataReader {
public:
    MtiDataReader();
    ~MtiDataReader();

    bool initialize(bool specificPort = false);
    bool configureDevice();
    bool openPort();

    bool startRecording();
    bool stopRecording();
    bool closeLogFile();
    void closePort();
    void freeControlObject();


    /*
     * acquisition des donnees
     */

    uint32_t getPacketCounter() const;
    XsVector getAcceleration() const;

private:

    static XsPortInfo initialize_specificPort(int portNumber);


    XsControl *_control = nullptr;
    XsPortInfo _mtPort;
    XsDevice *_device = nullptr;

};


#endif //MTI680_DATA_RECEIVER_MTIDATAREADER_H
