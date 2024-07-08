#include "managersignals.h"

managerSignals::managerSignals(QObject *parent) : QObject(parent)
{

}

managerSignals::~managerSignals()
{

}

void managerSignals::allConnect()
{
    try
    {
        SettingsToRlicontrol();
        SettingsToVideoControl();
    }
    catch (...)
    {
        qDebug() << "Error All Connects";
    }
}

QSharedPointer<managerSignals> managerSignals::create()
{
    static const QSharedPointer<managerSignals> manager = QSharedPointer<managerSignals>::create();

    return manager;
}

void managerSignals::SettingsToRlicontrol()
{
    if((_settings == nullptr) || (_rliControl == nullptr))
    {
        qDebug() << "ERROR CONNECT SettingsToRlicontrol";
        return;
    }

    connect(_settings, &SettingsWid::createNewRLI, _rliControl, &RLIControl::createNewRadar);
}

void managerSignals::SettingsToVideoControl()
{
    if((_settings == nullptr) || (_videoControl == nullptr))
    {
        qDebug() << "ERROR CONNECT SettingsToRlicontrol";
        return;
    }
    connect(_settings, &SettingsWid::createNewCamera, _videoControl, &videoControl::createNewCamera);
}
