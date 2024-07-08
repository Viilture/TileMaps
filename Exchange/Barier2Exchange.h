#ifndef VIDEOEXCHANGE_H
#define VIDEOEXCHANGE_H

#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>
#include <QTimer>
#include <QTime>
#include <QSharedPointer>
#include <QRandomGenerator>
#include <QDebug>

#include "ExchangeGlobal.h"
#include <Exchange/BiusTransport/cdgtransport.h>
#include <Exchange/BiusTransport/tcptransport.h>

Q_DECLARE_METATYPE(QSharedPointer<CmdSetMode>)
Q_DECLARE_METATYPE(QSharedPointer<RegistrData>)
Q_DECLARE_METATYPE(QSharedPointer<StateRLU>)
Q_DECLARE_METATYPE(QSharedPointer<RLUData1>)

enum modeExchange
{
    client = 1,
    server = 2,
};

/** @class  Класс обмена по TCP протоколу */
class Barier2Exchange : public QObject
{
    Q_OBJECT

    QSharedPointer<Cdg> tcpExchange;

    /** @brief  Режим работы класса, как сервер или клиент */
    modeExchange modeExch = modeExchange::client;

    /** @brief  ip addres */
    QHostAddress host = QHostAddress("127.0.0.1");
    /** @brief  port */
    int32_t port = 9999;
    /** @brief  номер отправителя */
    int32_t numSender = 178;
    /** @brief  номер получателя */
    int32_t numRequester = 56;

    /** @brief Создаем сокет */
    void createExchange();

public:
    explicit Barier2Exchange(QObject *parent = nullptr,
                             modeExchange newMode = modeExchange::server,
                             int32_t newPort = 9999,
                             QString newHost = "127.0.0.1",
                             int32_t newNumSender = 178,
                             int32_t newNumRequester = 56);
    virtual ~Barier2Exchange();


public Q_SLOTS:

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

    /** @brief Пришло сообщение */
    void readyRead();

Q_SIGNALS:

    void readyReadCmdSetMode(QSharedPointer<CmdSetMode> send);
    void readyReadRegistrData(QSharedPointer<RegistrData> send);
    void readyReadStateRLU(QSharedPointer<StateRLU> send);
    void readyReadRLUData1(QSharedPointer<RLUData1> send);

};

#endif // VIDEOEXCHANGE_H
