#include "testvideoexchange.h"

testVideoExchange::testVideoExchange(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<CmdSetState>("CmdSetIP");
}

testVideoExchange::~testVideoExchange()
{

}

/** @brief Тестируем класс обмена */
void testVideoExchange::testExchange()
{
    shpServer = QSharedPointer<VideoExchange>::create(this, modeExchange::server, 6189, "127.0.0.1");
    shpClient = QSharedPointer<VideoExchange>::create(this, modeExchange::client, 6189, "127.0.0.1");

    testExchangeTemp(CmdSetState());
    testExchangeTemp(CmdPowerOFF());
    testExchangeTemp(CmdRotateCamera());
    testExchangeTemp(CmdState());

}

/** @brief Тестируем класс обмена */
void testVideoExchange::testSerialize()
{
    testSerialiseTemp(CmdSetState());
    testSerialiseTemp(CmdPowerOFF());
    testSerialiseTemp(CmdRotateCamera());
    testSerialiseTemp(CmdState());
}

/** @brief Тестируем сериализацию и десереализацию структур обмена */
template<typename T>
void testVideoExchange::testSerialiseTemp(T type)
{
    qDebug() << "Begin Test Serialise - " << typeid(T).name();
    for(int i = 0; i < sizeTesting; i++)
    {
        T cdg = T::randomGen();

        QByteArray array = serialize(cdg);

        T cdg2;

        deserialize(array, cdg2);

        QCOMPARE(cdg, cdg2);
    }
}

/** @brief Тестируем обмен сообщениями по TCP */
template<typename T>
void testVideoExchange::testExchangeTemp(T type)
{
    qDebug() << "Begin Test Exchange - " << typeid(T).name();
    int32_t numSends = 0;

    if(std::is_same<T, CmdSetState>()) shpSpy = QSharedPointer<QSignalSpy>::create(shpServer.data(), &VideoExchange::readyReadCmdSetIP);
    else if(std::is_same<T, CmdPowerOFF>()) shpSpy = QSharedPointer<QSignalSpy>::create(shpServer.data(), &VideoExchange::readyReadCmdPowerOFF);
    else if(std::is_same<T, CmdRotateCamera>()) shpSpy = QSharedPointer<QSignalSpy>::create(shpServer.data(), &VideoExchange::readyReadCmdRotateCamera);
    else if(std::is_same<T, CmdState>()) shpSpy = QSharedPointer<QSignalSpy>::create(shpServer.data(), &VideoExchange::readyReadCmdState);
    shpSpy->wait(10);

    for(int i = 0; i < sizeTesting; i++)
    {
        T cdg = T::randomGen();

        sendCDG(cdg);

        QVERIFY(shpSpy->wait(50));

        QList<QVariant> arguments = shpSpy->takeFirst();
        QSharedPointer<T> cdg2 = qvariant_cast<QSharedPointer<T>>(arguments.at(0));

        QCOMPARE(cdg, *cdg2.data());
        if(cdg == *cdg2.data())
            numSends++;
    }

    qDebug() << "We have successfully sent and received " << numSends << " out of " << sizeTesting << " messages like " << typeid(T).name();
}

/** @brief отправляем сообщение по TCP */
template<>
void testVideoExchange::sendCDG<CmdSetState>(CmdSetState& type)
{
    if(shpClient.isNull()) return;

    QSharedPointer<CmdSetState> data = QSharedPointer<CmdSetState>::create(type);

    shpClient->sendCmdSetState(data);
}

/** @brief отправляем сообщение по TCP */
template<>
void testVideoExchange::sendCDG<CmdPowerOFF>(CmdPowerOFF& type)
{
    if(shpClient.isNull()) return;

    QSharedPointer<CmdPowerOFF> data = QSharedPointer<CmdPowerOFF>::create(type);

    shpClient->sendCmdPowerOFF(data);
}

/** @brief отправляем сообщение по TCP */
template<>
void testVideoExchange::sendCDG<CmdRotateCamera>(CmdRotateCamera& type)
{
    if(shpClient.isNull()) return;

    QSharedPointer<CmdRotateCamera> data = QSharedPointer<CmdRotateCamera>::create(type);

    shpClient->sendCmdRotateCamera(data);
}

/** @brief отправляем сообщение по TCP */
template<>
void testVideoExchange::sendCDG<CmdState>(CmdState& type)
{
    if(shpClient.isNull()) return;

    QSharedPointer<CmdState> data = QSharedPointer<CmdState>::create(type);

    shpClient->sendCmdState(data);
}
