#ifndef RLICONTROL_H
#define RLICONTROL_H

#pragma once

#include <QObject>
#include <QVector>
#include <QList>
#include <QSharedPointer>
#include <QStringListModel>
#include <QVariant>
#include <QMessageBox>

#include "scenecontrol.h"
#include "radarcontrol.h"
#include "managersignals.h"
#include "settingsBR.h"
#include "globalBarier2.h"

#define Barier2INI "/Barier2INI"

class managerSignals;
class RadarControl;


class RLIControl : public QStringListModel
{
    Q_OBJECT

    /** @brief Выбран радар */
    int32_t selectedRLI = -1;

    /** @brief Управление сигналами */
    QSharedPointer<managerSignals> manager;

    /** @brief Список радаров */
    QList<QSharedPointer<RadarControl>> listRadars;

    /** @brief Cсылка на класс управления сценой */
    sceneControl* scenControl = nullptr;

    /** @brief Класс настроек*/
    QSharedPointer<SettingsBR> set;

    /** @brief Восстановим все радары из ini файла */
    void rememberAllRadars();

public:
    explicit RLIControl(QObject *parent = nullptr, sceneControl* newScenControl = nullptr);
    virtual ~RLIControl();

    /** @brief Получение IP4 */
    QString getIp4(int32_t numRLI);
    /** @brief Установка IP4 */
    void setIp4(int32_t numRLI, QString ip4);
    /** @brief Получение литеры */
    int32_t getLitera(int32_t numRLI);
    /** @brief Установка литеры */
    void setLitera(int32_t numRLI, int32_t litera);
    /** @brief Получение режима работы */
    modeRLI getMode(int32_t numRLI);
    /** @brief Установка режима работы */
    void setMode(int32_t numRLI, modeRLI mode);
    double getAzim(int32_t numRLI) const;
    void setAzim(int32_t numRLI, double newAzim);
    double getTang(int32_t numRLI) const;
    void setTang(int32_t numRLI, double newTang);
    double getKren(int32_t numRLI) const;
    void setKren(int32_t numRLI, double newKren);
    double getLat(int32_t numRLI) const;
    void setLat(int32_t numRLI, double newLat);
    double getLong(int32_t numRLI) const;
    void setLong(int32_t numRLI, double newLong);
    QString getName(int32_t numRLI) const;
    void setName(int32_t numRLI, QString newName);
    int32_t getRadarSize();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

public Q_SLOTS:

    /** @brief Создаем новый радар */
    void createNewRadar();

    /** @brief Создаем новый радар c заранее известными параметрами*/
    void createRadar(double lat  = 0.0,
                     double longit = 0.0,
                     double azim = 0.0,
                     double tang = 0.0,
                     double kren = 0.0,
                     QString ip4 = "127.0.0.1",
                     modeRLI mode = modeRLI::air,
                     int32_t litera = 1, QString nameRli = QLatin1String(""));

    /** @brief Удаляем радар */
    void removeRadar(int32_t numRLI);

    /** @brief Сохраняем данные в Ini файл */
    void saveToIni();

    /** @brief Выбрать Радар */
    void selectRadar(int32_t numRLI);

};

#endif // RLICONTROL_H
