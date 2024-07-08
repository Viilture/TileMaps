#include "tcptransport.h"
#include <QTcpServer>
#include <QTcpSocket>

QString Transport::socketString(QTcpSocket *tcpSocket)
{
    QString str = QString("[%1:%2]").arg(tcpSocket->peerAddress().toString()).arg(tcpSocket->peerPort());
    return str;

}

TcpServerTransport::TcpServerTransport(quint16 port, QHostAddress hostAddress, int writeLimit) : writeLimit(writeLimit)
{
    tcpServer = QSharedPointer<QTcpServer>::create();

    tcpServer->listen(hostAddress, port);
    if (tcpServer->isListening() == false)
    {
        qDebug() << tcpServer->errorString();
        exit(1);
    }
    connect(tcpServer.data(), SIGNAL(newConnection()), SLOT(onNewConnection()));

}

bool TcpServerTransport::write(QByteArray ba)
{
    int ret = false;
    foreach(auto socket, socketList)
    {
        if (socket->state() != QTcpSocket::ConnectedState)
        {
            socket->abort();
            continue;
        }

        if (socket->bytesToWrite() > writeLimit)
        {
            socket->abort();
            continue;
        }
        socket->write(ba);
        ret = true;
    }
    return ret;
}


void TcpServerTransport::onNewConnection()
{
    while(tcpServer->hasPendingConnections())
    {
        QSharedPointer<QTcpSocket> tcpSocket = QSharedPointer<QTcpSocket>::create(tcpServer->nextPendingConnection());
        connect(tcpSocket.data(), SIGNAL(disconnected()),SLOT(onDisconnected()));
        connect(tcpSocket.data(), SIGNAL(readyRead()),SLOT(onSocketRead()));
        if (socketList.size() == 0)
            emit connected();

        socketList.append(tcpSocket);
        qDebug() << "Connected:" << socketString(tcpSocket.data());
    }

}

void TcpServerTransport::onDisconnected()
{
    QSharedPointer<QTcpSocket> tcpSocket = QSharedPointer<QTcpSocket>::create((QTcpSocket*)sender());
    qDebug() << "Disconnected:" << socketString(tcpSocket.data());
    socketList.removeAll(tcpSocket);
    dataMap[tcpSocket].clear();
    tcpSocket->deleteLater();
    if (socketList.size() == 0)
        emit disconnected();

}

void TcpServerTransport::onSocketRead()
{

    QSharedPointer<QTcpSocket> tcpSocket = QSharedPointer<QTcpSocket>::create((QTcpSocket*)sender());

    QByteArray& ba = dataMap[tcpSocket];
    ba += tcpSocket->readAll();

    emit readyRead(&ba);
}

////////////////////////////////////////////////////////////////////////////////////////

TcpClientTransport::TcpClientTransport(quint16 port, QHostAddress hostAddress, int writeLimit)
    : port(port), hostAddress(hostAddress), writeLimit(writeLimit)
{
    tcpSocket = QSharedPointer<QTcpSocket>::create();

    tcpSocket->connectToHost(hostAddress, port);
    reconnectTimerId = startTimer(1000);
    connect(tcpSocket.data(), SIGNAL(connected()), SLOT(onNewConnection()));
    connect(tcpSocket.data(), SIGNAL(connected()), SIGNAL(connected()));
    connect(tcpSocket.data(), SIGNAL(disconnected()),SLOT(onDisconnected()));
    connect(tcpSocket.data(), SIGNAL(disconnected()),SIGNAL(disconnected()));
    connect(tcpSocket.data(), SIGNAL(readyRead()),SLOT(onSocketRead()));
    connect(tcpSocket.data(), SIGNAL(error( QAbstractSocket::SocketError)),SLOT(onError( QAbstractSocket::SocketError)));


}

bool TcpClientTransport::write(QByteArray ba)
{
    if (tcpSocket->bytesToWrite() > writeLimit)
    {
        tcpSocket->abort();
        return false;
    }

    if (tcpSocket->state() == QTcpSocket::ConnectedState)
    {
        tcpSocket->write(ba);
        return true;
    }
    return false;

}

void TcpClientTransport::abort()
{
    tcpSocket->abort();
}

void TcpClientTransport::onNewConnection()
{
    buffer.clear();
    killTimer(reconnectTimerId);
}

void TcpClientTransport::onDisconnected()
{
    reconnectTimerId = startTimer(1000);
}

void TcpClientTransport::onError(QAbstractSocket::SocketError /*socketError*/)
{
    qDebug() << tcpSocket->errorString();
    emit error(tcpSocket->error());
    tcpSocket->abort();

}

void TcpClientTransport::timerEvent(QTimerEvent *)
{
    if (tcpSocket->state() != QTcpSocket::ConnectedState)
        tcpSocket->connectToHost(hostAddress, port);

}

void TcpClientTransport::onSocketRead()
{
    buffer += tcpSocket->readAll();
    emit readyRead(&buffer);
}


