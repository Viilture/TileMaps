#include "cdgtransport.h"
#include "tcptransport.h"

Cdg::Cdg(QSharedPointer<Transport> newTransport)
{
    transport = newTransport;

    connect(transport.data(), SIGNAL(readyRead(QByteArray*)), SLOT(onReadyRead(QByteArray*)));
}


bool Cdg::writeCdg(QByteArray cdg)
{
    return transport->write(cdg);
}

bool Cdg::readCdg(QByteArray *cdg)
{
    if (cdgList.isEmpty() == false)
    {
        *cdg = cdgList.first();
        cdgList.removeFirst();
        return true;
    }
    return false;
}

void Cdg::abort()
{

}

void Cdg::onReadyRead(QByteArray* buffer)
{

    while(1)
    {
        int size = buffer->size();

        //Проверка размера буфера
        char* data = buffer->data();
        QByteArray cdg(data, size);
        buffer->remove(0, size);
        cdgList.append(cdg);

        emit readyRead();
        return;

    }
}
////////////////////////////////////////////////////////////////////////////////////////

CdgServer::CdgServer(quint16 port, QHostAddress hostAddress) :
    Cdg(QSharedPointer<Transport>(new TcpServerTransport(port, hostAddress)))
{
    connect(transport.data(), SIGNAL(connected()), SIGNAL(connected()));
    connect(transport.data(), SIGNAL(disconnected()),SIGNAL(disconnected()));
}

void CdgServer::abort()
{

}

CdgClient::CdgClient(quint16 port, QString hostAddress) :
    Cdg(QSharedPointer<Transport>(new TcpClientTransport(port, QHostAddress(hostAddress))))
{
    connect(transport.data(), SIGNAL(connected()), SIGNAL(connected()));
    connect(transport.data(), SIGNAL(disconnected()),SIGNAL(disconnected()));
    connect(transport.data(),SIGNAL(error(QAbstractSocket::SocketError)),SIGNAL(error(QAbstractSocket::SocketError)));
}

void CdgClient::abort()
{
    transport->abort();
}
