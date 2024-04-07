/**
 * Classe MtiDataReader
 * @author Louis Jeauffroy
 * @date 04/04/2024
 * Description :
 *      Classe permettant d'aller récupérer les données utiles au projet via l'extraction de celle-ci dans le fichier
 *      LOG créé après l'enregistrement des mesures du capteur Mti-680G. Elle utilise pour cela les méthodes de
 *      Xsens Device API.
 *      Les données lues sont ensuite stockées dans les attributs d'une instance de la classe MtiDataValues
 */

#ifndef MTI680_DATA_RECEIVER_MTIPARSER_H
#define MTI680_DATA_RECEIVER_MTIPARSER_H

#include <xsensdeviceapi.h>
#include <xstypes/xstime.h>
#include <xscommon/xsens_mutex.h>

#include "MtiDataValues.h"

#include <iostream>
#include <fstream>

class CallbackHandler_Parser : public XsCallback
{
public:
    CallbackHandler_Parser()
            : m_progress(0)
    {
    }

    virtual ~CallbackHandler_Parser() throw()
    {
    }

    int progress() const
    {
        return m_progress;
    }
protected:
    void onProgressUpdated(XsDevice* dev, int current, int total, const XsString* identifier) override
    {
        (void)dev;
        (void)total;
        (void)identifier;
        m_progress = current;
    }
private:
    volatile std::atomic_int m_progress;
};

class MtiParser {
public:
    MtiParser();
    ~MtiParser();

    bool openLogFile();
    void createDeviceInstance();
    void loadLogFile();
    void exportData();

    MtiDataValues getValues();

    XsControl getControl() const;
    CallbackHandler_Parser &getCallbackHandler();

private:

    XsControl *_control = nullptr;
    XsDevice *_device = nullptr;
    CallbackHandler_Parser _callbackHandler;
    MtiDataValues _dataValues;
};


#endif //MTI680_DATA_RECEIVER_MTIPARSER_H
