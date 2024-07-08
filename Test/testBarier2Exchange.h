#ifndef TESTBARIER2EXCHANGE_H
#define TESTBARIER2EXCHANGE_H

#include <QObject>
#include <QTest>
#include <QRandomGenerator>
#include <QSignalSpy>
#include <QThread>

#include "ExchangeGlobal.h"
#include "Barier2Exchange.h"


/** @class Класс тестирования обмен */
class testBarier2Exchange : public QObject
{
    Q_OBJECT

    QSharedPointer<Barier2Exchange> shpServer;
    QSharedPointer<QSignalSpy> shpSpy;
    QSharedPointer<Barier2Exchange> shpClient;

    /** @brief Количество пройденных тестов */
    int32_t sizeTesting = 20000;

    /** @brief Тестирование сериализации */
    template<typename T>
    void testSerialiseTemp(T type);

    /** @brief Тестирование обмена */
    template<typename T>
    void testExchangeTemp(T type);

    /** @brief Шаблон отправки кодограммы */
    template<typename T>
    void sendCDG(T& type);

public:
    explicit testBarier2Exchange(QObject *parent = nullptr);
    virtual ~testBarier2Exchange();

private Q_SLOTS:

    /** @brief Тестирование сериализации*/
    void testSerialize();

    /** @brief Тестирование обмена */
    void testExchange();

};

#endif // TESTBARIER2EXCHANGE_H
