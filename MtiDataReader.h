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

class CallbackHandler_Reader : public XsCallback
{
public:
    CallbackHandler_Reader(size_t maxBufferSize = 5)
            : m_maxNumberOfPacketsInBuffer(maxBufferSize)
            , m_numberOfPacketsInBuffer(0)
    {
    }

    virtual ~CallbackHandler_Reader() throw()
    {
    }

    bool packetAvailable() const
    {
        xsens::Lock locky(&m_mutex);
        return m_numberOfPacketsInBuffer > 0;
    }

    XsDataPacket getNextPacket()
    {
        assert(packetAvailable());
        xsens::Lock locky(&m_mutex);
        XsDataPacket oldestPacket(m_packetBuffer.front());
        m_packetBuffer.pop_front();
        --m_numberOfPacketsInBuffer;
        return oldestPacket;
    }

protected:
    virtual void onLiveDataAvailable(XsDevice*, const XsDataPacket* packet)
    {
        xsens::Lock locky(&m_mutex);
        assert(packet != nullptr);
        while (m_numberOfPacketsInBuffer >= m_maxNumberOfPacketsInBuffer)
            (void)getNextPacket();

        m_packetBuffer.push_back(*packet);
        ++m_numberOfPacketsInBuffer;
        assert(m_numberOfPacketsInBuffer <= m_maxNumberOfPacketsInBuffer);
    }
private:
    mutable xsens::Mutex m_mutex;

    size_t m_maxNumberOfPacketsInBuffer;
    size_t m_numberOfPacketsInBuffer;
    std::list<XsDataPacket> m_packetBuffer;
};

class MtiDataReader {

public:
    MtiDataReader();
    ~MtiDataReader();

    bool init(bool specificPort = false);
    bool configureDevice();
    bool createLogFile();
    bool openPort();

    bool startRecording();
    bool stopRecording();
    bool closeLogFile();
    void closePort();
    void freeControlObject();

    XsControl getControl() const;
    CallbackHandler_Reader &getCallbackHandler();

private:

    static XsPortInfo initialize_specificPort(int portNumber);


    XsControl *_control = nullptr;
    XsPortInfo _mtPort;
    XsDevice *_device = nullptr;
    CallbackHandler_Reader _callbackHandler;

};


#endif //MTI680_DATA_RECEIVER_MTIDATAREADER_H
