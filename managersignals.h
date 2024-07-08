#ifndef MANAGERSIGNALS_H
#define MANAGERSIGNALS_H

#pragma once

#include <QObject>
#include <QSharedPointer>
#include "settingswid.h"
#include "rlicontrol.h"
#include "mapcontrol.h"
#include "videocontrol.h"

class SettingsWid;
class RLIControl;
class videoControl;

/** @brief Класс управления всеми соединенями программы*/
class managerSignals : public QObject
{
    Q_OBJECT

    SettingsWid*     _settings     = nullptr;
    RLIControl*   _rliControl   = nullptr;
    MapControl*   _MapControl   = nullptr;
    videoControl* _videoControl = nullptr;

    void SettingsToRlicontrol();
    void SettingsToMapControl();
    void SettingsToVideoControl();

public:
    explicit managerSignals(QObject *parent = nullptr);
    virtual ~managerSignals();

    void allConnect();

    static QSharedPointer<managerSignals> create();

public Q_SLOTS:

    /** @brief Виджет настроек*/
    void addWidSettings(SettingsWid* settings)
    {if(settings != nullptr) _settings = settings;};

    /** @brief Виджет управления РЛУ*/
    void addRLIControl(RLIControl* rliControl)
    {if(rliControl != nullptr) _rliControl = rliControl;};

    /** @brief Виджет управления картой*/
    void addMapControl(MapControl* newMapControl)
    {if(newMapControl != nullptr) _MapControl = newMapControl;};
    /** @brief Виджет управления картой*/
    void addVideoControl(videoControl* newVideoControl)
    {if(newVideoControl != nullptr) _videoControl = newVideoControl;};

};

#endif // MANAGERSIGNALS_H
