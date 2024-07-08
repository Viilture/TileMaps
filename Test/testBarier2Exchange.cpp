#include "testBarier2Exchange.h"


testBarier2Exchange::testBarier2Exchange(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<CmdSetMode>("CmdSetMode");
    qRegisterMetaType<RegistrData>("RegistrData");
    qRegisterMetaType<StateRLU>("StateRLU");
    qRegisterMetaType<RLUData1>("RLUData1");
}

testBarier2Exchange::~testBarier2Exchange()
{

}

/** @brief Тестируем класс обмена */
void testBarier2Exchange::testExchange()
{
    shpServer = QSharedPointer<Barier2Exchange>::create(this, modeExchange::server, 61899, "127.0.0.1");
    shpClient = QSharedPointer<Barier2Exchange>::create(this, modeExchange::client, 61899, "127.0.0.1");

    testExchangeTemp(CmdSetMode());
    testExchangeTemp(RegistrData());
    testExchangeTemp(StateRLU());
    testExchangeTemp(RLUData1());
}

/** @brief Тестируем класс обмена */
void testBarier2Exchange::testSerialize()
{
    testSerialiseTemp(CmdSetMode());
    testSerialiseTemp(RegistrData());
    testSerialiseTemp(StateRLU());
    testSerialiseTemp(TargetData1());
    testSerialiseTemp(RLUData1());
}

/** @brief Тестируем сериализацию и десереализацию структур обмена */
template<typename T>
void testBarier2Exchange::testSerialiseTemp(T type)
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
void testBarier2Exchange::testExchangeTemp(T type)
{
    qDebug() << "Begin Test Exchange - " << typeid(T).name();
    int32_t numSends = 0;

    if(std::is_same<T, CmdSetMode>())       shpSpy = QSharedPointer<QSignalSpy>::create(shpServer.data(), &Barier2Exchange::readyReadCmdSetMode);
    else if(std::is_same<T, RegistrData>()) shpSpy = QSharedPointer<QSignalSpy>::create(shpServer.data(), &Barier2Exchange::readyReadRegistrData);
    else if(std::is_same<T, StateRLU>())    shpSpy = QSharedPointer<QSignalSpy>::create(shpServer.data(), &Barier2Exchange::readyReadStateRLU);
    else if(std::is_same<T, RLUData1>())    shpSpy = QSharedPointer<QSignalSpy>::create(shpServer.data(), &Barier2Exchange::readyReadRLUData1);

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
void testBarier2Exchange::sendCDG<CmdSetMode>(CmdSetMode& type)
{
    if(shpClient.isNull()) return;

    QSharedPointer<CmdSetMode> data = QSharedPointer<CmdSetMode>::create(type);

    shpClient->sendCmdSetMode(data);
}

/** @brief отправляем сообщение о регистрации данных с РЛС */
template<>
void testBarier2Exchange::sendCDG<RegistrData>(RegistrData& type)
{
    if(shpClient.isNull()) return;

    QSharedPointer<RegistrData> data = QSharedPointer<RegistrData>::create(type);

    shpClient->sendRegistrData(data);
}

/** @brief отправляем сообщение о регистрации данных с РЛС */
template<>
void testBarier2Exchange::sendCDG<StateRLU>(StateRLU& type)
{
    if(shpClient.isNull()) return;

    QSharedPointer<StateRLU> data = QSharedPointer<StateRLU>::create(type);

    shpClient->sendStateRLU(data);
}

/** @brief отправляем сообщение о регистрации данных с РЛС */
template<>
void testBarier2Exchange::sendCDG<RLUData1>(RLUData1& type)
{
    if(shpClient.isNull()) return;

    QSharedPointer<RLUData1> data = QSharedPointer<RLUData1>::create(type);

    shpClient->sendRLUData1(data);
}
