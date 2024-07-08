#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QObject>
#include <QCoreApplication>
#include <QDebug>

#define BRINI "/Barier2.ini"

class SettingsBR : public QSettings
{
    Q_OBJECT

public:
    explicit SettingsBR(QObject *parent = nullptr);
    virtual ~SettingsBR();
};


#endif // SETTINGS_H
