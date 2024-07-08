#include "videoexchange.h"

VideoExchange::VideoExchange(QObject *parent,
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

    switch (modeExch)
    {
        case modeExchange::client:
        {
            tcpExchange = QSharedPointer<CdgClient>::create(port, host);
            break;
        }
        case modeExchange::server:
        {
            tcpExchange = QSharedPointer<CdgServer>::create(port, QHostAddress(host));
            break;
        }
        default:
            break;
    }

    connect(tcpExchange.data(), &Cdg::readyRead, this, &VideoExchange::readyRead);
}

VideoExchange::~VideoExchange()
{

}

/** @class Команда установки параметров модуля видео */
void VideoExchange::sendCmdSetState(QSharedPointer<CmdSetState> send)
{
    if(tcpExchange.isNull()) return;

    //    send->head.size = SIZE_CmdSetMode;
    //    send->head.type = typeCDGVideoExch::setModeRLU;
    //    send->head.numSender = numSender;
    //    send->head.numRequester = numRequester;
    //    send->head.time = QTime::currentTime().hour();

    QByteArray array = serialize(*send.data());

    tcpExchange->writeCdg(array);
}

 /** @class Выключение модуля Видео */
void VideoExchange::sendCmdPowerOFF(QSharedPointer<CmdPowerOFF> send)
{
    if(tcpExchange.isNull()) return;

    //    send->head.size = SIZE_CmdSetMode;
    //    send->head.type = typeCDGVideoExch::setModeRLU;
    //    send->head.numSender = numSender;
    //    send->head.numRequester = numRequester;
    //    send->head.time = QTime::currentTime().hour();

    QByteArray array = serialize(*send.data());

    tcpExchange->writeCdg(array);
}

/** @class Повернуть камеру по сферическим координатам */
void VideoExchange::sendCmdRotateCamera(QSharedPointer<CmdRotateCamera> send)
{
    if(tcpExchange.isNull()) return;

    //    send->head.size = SIZE_CmdSetMode;
    //    send->head.type = typeCDGVideoExch::setModeRLU;
    //    send->head.numSender = numSender;
    //    send->head.numRequester = numRequester;
    //    send->head.time = QTime::currentTime().hour();

    QByteArray array = serialize(*send.data());

    tcpExchange->writeCdg(array);
}

/** @class Текущее состояние модуля камеры */
void VideoExchange::sendCmdState(QSharedPointer<CmdState> send)
{
    if(tcpExchange.isNull()) return;

    //    send->head.size = SIZE_CmdSetMode;
    //    send->head.type = typeCDGVideoExch::setModeRLU;
    //    send->head.numSender = numSender;
    //    send->head.numRequester = numRequester;
    //    send->head.time = QTime::currentTime().hour();

    QByteArray array = serialize(*send.data());

    tcpExchange->writeCdg(array);
}

void VideoExchange::readyRead()
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
        case typeCDGVideoExch::setIP:
        {
            CmdSetState cdg;
            deserialize(array, cdg);

            QSharedPointer<CmdSetState> data = QSharedPointer<CmdSetState>::create(cdg);

            Q_EMIT readyReadCmdSetIP(data);
            break;
        }
        case typeCDGVideoExch::powerOFF:
        {
            CmdPowerOFF cdg;
            deserialize(array, cdg);

            QSharedPointer<CmdPowerOFF> data = QSharedPointer<CmdPowerOFF>::create(cdg);

            Q_EMIT readyReadCmdPowerOFF(data);
            break;
        }
        case typeCDGVideoExch::rotateCamera:
        {
            CmdRotateCamera cdg;
            deserialize(array, cdg);

            QSharedPointer<CmdRotateCamera> data = QSharedPointer<CmdRotateCamera>::create(cdg);

            Q_EMIT readyReadCmdRotateCamera(data);
            break;
        }
        case typeCDGVideoExch::stateCamera:
        {
            CmdState cdg;
            deserialize(array, cdg);

            QSharedPointer<CmdState> data = QSharedPointer<CmdState>::create(cdg);

            Q_EMIT readyReadCmdState(data);
            break;
        }
        default:
        {
            break;
        }
        }
    }
}
