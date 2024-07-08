#include "Barier2Exchange.h"

Barier2Exchange::Barier2Exchange(QObject *parent,
                                 modeExchange newMode,
                                 int32_t newPort,
                                 QString newHost,
                                 int32_t newNumSender,
                                 int32_t newNumRequester) : QObject(parent)
{
    numSender = newNumSender;
    numRequester = newNumRequester;
    modeExch = newMode;
    port = newPort;
    host = newHost;

    createExchange();

    // static QTimer timer;

    // connect(&timer, &QTimer::timeout, this, [=](){
    //     tcpExchange->writeCdg(QByteArray::fromStdString("Hello"));
    // });

    // connect(tcpExchange, &Cdg::readyRead, this, &Rls4332Exchange::readyRead);
    // //  timer.start(200);
}

Barier2Exchange::~Barier2Exchange()
{

}

/** @brief Создаем сокет */
void Barier2Exchange::createExchange()
{
    QHostAddress eHost = QHostAddress(host);

    if((!host.isGlobal()) && (!host.isLinkLocal())) return;

    if(host.isNull() || (port < 1)) return;

    if(!tcpExchange.isNull())
    {
        disconnect(tcpExchange.data(), &Cdg::readyRead, this, &Barier2Exchange::readyRead);
        tcpExchange.reset();
    }

    switch (modeExch)
    {
    case modeExchange::client:
    {
        tcpExchange = QSharedPointer<CdgClient>::create(port, host.toString());
        break;
    }
    case modeExchange::server:
    {
        tcpExchange = QSharedPointer<CdgServer>::create(port, host);
        break;
    }
    default:
        break;
    }

    connect(tcpExchange.data(), &Cdg::readyRead, this, &Barier2Exchange::readyRead);
}

/** @brief Меняем хост */
void Barier2Exchange::setHost(QString newHost)
{
    QHostAddress newHostAddr = QHostAddress(newHost);
    if((host == newHostAddr) || ((!newHostAddr.isGlobal()) && (!newHostAddr.isLinkLocal()))) return;

        host = newHostAddr;
        createExchange();

}

/** @brief Меняем порт */
void Barier2Exchange::setPort(int32_t newPort)
{
    if(port != newPort)
    {
        port = newPort;

        createExchange();
    }
}

/** @brief Отправка команды установления настроек СВОИ и РЛС */
void Barier2Exchange::sendCmdSetMode(QSharedPointer<CmdSetMode> send)
{
    if(tcpExchange.isNull()) return;

    //    send->head.size = SIZE_CmdSetMode;
    //    send->head.type = typeCDGBarier2Exch::setModeRLU;
    //    send->head.numSender = numSender;
    //    send->head.numRequester = numRequester;
    //    send->head.time = QTime::currentTime().hour();

    QByteArray array = serialize(*send.data());

    tcpExchange->writeCdg(array);
}

/** @brief Отправка данных регистрации с РЛС */
void Barier2Exchange::sendRegistrData(QSharedPointer<RegistrData> send)
{
    if(tcpExchange.isNull()) return;

    //    send->head.size = SIZE_CmdSetMode;
    //    send->head.type = typeCDGBarier2Exch::setModeRLU;
    //    send->head.numSender = numSender;
    //    send->head.numRequester = numRequester;
    //    send->head.time = QTime::currentTime().hour();

    QByteArray array = serialize(*send.data());

    tcpExchange->writeCdg(array);
}

/** @brief Отправка состояния РЛУ с АРМа */
void Barier2Exchange::sendStateRLU(QSharedPointer<StateRLU> send)
{
    if(tcpExchange.isNull()) return;

    //    send->head.size = SIZE_CmdSetMode;
    //    send->head.type = typeCDGBarier2Exch::setModeRLU;
    //    send->head.numSender = numSender;
    //    send->head.numRequester = numRequester;
    //    send->head.time = QTime::currentTime().hour();

    QByteArray array = serialize(*send.data());

    tcpExchange->writeCdg(array);
}

/** @brief Отправка  Данных о целях с РЛУ */
void Barier2Exchange::sendRLUData1(QSharedPointer<RLUData1> send)
{
    if(tcpExchange.isNull()) return;

    //    send->head.size = SIZE_CmdSetMode;
    //    send->head.type = typeCDGBarier2Exch::setModeRLU;
    //    send->head.numSender = numSender;
    //    send->head.numRequester = numRequester;
    //    send->head.time = QTime::currentTime().hour();

    QByteArray array = serialize(*send.data());

    tcpExchange->writeCdg(array);
}

void Barier2Exchange::readyRead()
{
    while(tcpExchange->isCdgAvailable())
    {
        QByteArray array;

        bool stCDG = tcpExchange->readCdg(&array);

        //Проверяем, что переданное сообщение может в себе вместить заголовок кодограммы
        if(!stCDG)                    { qDebug() << "!stCDG"; continue;}
        if(array.size() < SIZE_HEADER){ qDebug() << "array.size() = " << array.size() << "< HEADER_SIZE"; continue;}
        if(array.isEmpty())           { qDebug() << "array.isEmpty()"; continue;}

        HEADER headCDG;

        //Теперь пытаемся извлечь заголовок
        deserialize(array, headCDG);

        switch (headCDG.type)
        {
        case typeCDGBarier2Exch::setModeRLU:
        {
            CmdSetMode cdg;
            deserialize(array, cdg);

            QSharedPointer<CmdSetMode> data = QSharedPointer<CmdSetMode>::create(cdg);

            Q_EMIT readyReadCmdSetMode(data);
            break;
        }
        case typeCDGBarier2Exch::stateRLU:
        {
            StateRLU cdg;
            deserialize(array, cdg);

            QSharedPointer<StateRLU> data = QSharedPointer<StateRLU>::create(cdg);

            Q_EMIT readyReadStateRLU(data);
            break;
        }
        case typeCDGBarier2Exch::RegistrationDATA:
        {
            RegistrData cdg;
            deserialize(array, cdg);

            QSharedPointer<RegistrData> data = QSharedPointer<RegistrData>::create(cdg);

            Q_EMIT readyReadRegistrData(data);

            break;
        }
        case typeCDGBarier2Exch::dataTarget1:
        {
            RLUData1 cdg;
            deserialize(array, cdg);

            QSharedPointer<RLUData1> data = QSharedPointer<RLUData1>::create(cdg);

            Q_EMIT readyReadRLUData1(data);
            break;
        }
        default:
        {
            break;
        }
        }
    }
}
