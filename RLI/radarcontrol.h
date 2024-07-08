#ifndef RADARCONTROL_H
#define RADARCONTROL_H

#include <QObject>
#include <QWeakPointer>
#include <QPointF>
#include <QHostAddress>

#include "scenecontrol.h"
#include "mapradar.h"
#include "Exchange/Barier2Exchange.h"

class Barier2Exchange;

//Режим работы радара
enum modeRLI
{
    air   = 0, //Воздух
    earth = 1, //Земля
};


class RadarControl : public QObject
{
    Q_OBJECT

    /** @brief Cсылка на класс управления сценой */
    sceneControl* scenControl = nullptr;

    /** @brief Отображение радара */
    QSharedPointer<mapRadar> viewRLI;

    /** @brief Сетевой обмен с РЛУ */
    QSharedPointer<Barier2Exchange> BrExchange;

    QThread threadExchange;

    /** @brief Координата центра радара в WGS84 */
    QPointF coordWGS84 = QPointF(0, 0);

    /** @brief Ориентация по азимуту, тангажу и крену */
    double azim = 0.0,
    tang = 0.0,
    kren = 0.0;

    /** @brief Адрес РЛУ */
    QString ip4 = "127.0.0.1";

    /** @brief Режим работы */
    modeRLI mode = modeRLI::air;

    /** @brief Имя радара */
    QString name = QStringLiteral("Радар");

    /** @brief Литера */
    uint32_t litera = 1;

    void init();

    /** @brief Создаем визуализацию радара на сцене */
    void createViewRLI();

public:
    explicit RadarControl(QObject *parent = nullptr, sceneControl* newScenControl = nullptr);
    virtual  ~RadarControl();

    QPointF getCoordWGS84() const;
    void setCoordWGS84(QPointF newCoordWGS84);

    double getAzim() const;
    void setAzim(double newAzim);

    double getTang() const;
    void setTang(double newTang);

    double getKren() const;
    void setKren(double newKren);

    const QString &getIp4() const;
    void setIp4(const QString &newIp4);

    modeRLI getMode() const;
    void setMode(modeRLI newMode);

    uint32_t getLitera() const;
    void setLitera(uint32_t newLitera);

    void setLat(double newLat);
    void setLong(double newLong);

    const QString &getName() const;
    void setName(const QString &newName);

    /** @brief центрировать карту */
    void centerMap();

    /** @brief Помигать радаром */
    void blinked();


Q_SIGNALS:

    /** @brief Меняем хост */
    void setHost(QString newHost);

    /** @brief Меняем порт */
    void setPort(int32_t newPort);

    /** @brief Отправка команды установления настроек СВОИ и РЛУ */
    void sendCmdSetMode(QSharedPointer<CmdSetMode> send);

    /** @brief Отправка данных регистрации с РЛУ */
    void sendRegistrData(QSharedPointer<RegistrData> send);

    /** @brief Отправка состояния РЛУ с АРМа */
    void sendStateRLU(QSharedPointer<StateRLU> send);

    /** @brief Отправка  Данных о целях с РЛУ */
    void sendRLUData1(QSharedPointer<RLUData1> send);

};

#endif // RADARCONTROL_H
