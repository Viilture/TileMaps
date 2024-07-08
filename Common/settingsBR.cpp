#include "settingsBR.h"

SettingsBR::SettingsBR(QObject *parent) :
    QSettings(QCoreApplication::applicationDirPath() + BRINI,
              QSettings::IniFormat,
              parent)
{

}

SettingsBR::~SettingsBR()
{

}
