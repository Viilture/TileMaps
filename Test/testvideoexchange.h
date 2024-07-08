#ifndef TESTVIDEOEXCHANGE_H
#define TESTVIDEOEXCHANGE_H

#include <QObject>
#include <QTest>
#include <QRandomGenerator>
#include <QSignalSpy>
#include <QThread>

#include "videoexchange.h"
#include "ExchangeGlobal.h"

class VideoExchange;

/** @class Класс тестирования обмен */
class testVideoExchange : public QObject
{
    Q_OBJECT

    QSharedPointer<VideoExchange> shpServer;
    QSharedPointer<QSignalSpy> shpSpy;
    QSharedPointer<VideoExchange> shpClient;

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
    explicit testVideoExchange(QObject *parent = nullptr);
    virtual ~testVideoExchange();

private Q_SLOTS:

    /** @brief Тестирование сериализации*/
    void testSerialize();

    /** @brief Тестирование обмена */
    void testExchange();

};
#endif // TESTVIDEOEXCHANGE_H
