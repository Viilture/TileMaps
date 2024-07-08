#ifndef VIDEOEXCHANGE_H
#define VIDEOEXCHANGE_H

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QTimer>
#include <QTime>
#include <QSharedPointer>
#include <QRandomGenerator>
#include <QDebug>

#include "ExchangeGlobal.h"
#include <Exchange/BiusTransport/cdgtransport.h>
#include <Exchange/BiusTransport/tcptransport.h>
#include "settingsBR.h"

Q_DECLARE_METATYPE(QSharedPointer<CmdSetState>)
Q_DECLARE_METATYPE(QSharedPointer<CmdPowerOFF>)
Q_DECLARE_METATYPE(QSharedPointer<CmdRotateCamera>)
Q_DECLARE_METATYPE(QSharedPointer<CmdState>)

enum modeExchange
{
    client = 1,
    server = 2,
};

/** @class  Класс обмена по TCP протоколу */
class VideoExchange : public QObject
{
    Q_OBJECT

    QSharedPointer<Cdg> tcpExchange;

    /** @brief  Режим работы класса, как сервер или клиент */
    modeExchange modeExch = modeExchange::client;

    /** @brief  ip addres */
    QString host = "127.0.0.1";
    /** @brief  port */
    int32_t port = 9999;
    /** @brief  номер отправителя */
    int32_t numSender = 178;
    /** @brief  номер получателя */
    int32_t numRequester = 56;

public:
    explicit VideoExchange(QObject *parent = nullptr,
                           modeExchange newMode = modeExchange::server,
                           int32_t newPort = 9999,
                           QString newHost = "127.0.0.1",
                           int32_t newNumSender = 178,
                           int32_t newNumRequester = 56);

    virtual ~VideoExchange();

public Q_SLOTS:

    /** @class Команда установки параметров модуля видео */
    void sendCmdSetState(QSharedPointer<CmdSetState> send);
    /** @class Выключение модуля Видео */
    void sendCmdPowerOFF(QSharedPointer<CmdPowerOFF> send);
    /** @class Повернуть камеру по сферическим координатам */
    void sendCmdRotateCamera(QSharedPointer<CmdRotateCamera> send);
    /** @class Текущее состояние модуля камеры */
    void sendCmdState(QSharedPointer<CmdState> send);

    /** @brief Пришло сообщение */
    void readyRead();

Q_SIGNALS:

    void readyReadCmdSetIP(QSharedPointer<CmdSetState> send);
    void readyReadCmdPowerOFF(QSharedPointer<CmdPowerOFF> send);
    void readyReadCmdRotateCamera(QSharedPointer<CmdRotateCamera> send);
    void readyReadCmdState(QSharedPointer<CmdState> send);

};

#endif // VIDEOEXCHANGE_H
