#ifndef TCPTRANSPORT_H
#define TCPTRANSPORT_H

#include <QObject>
#include <QHostAddress>
#include <QSharedPointer>

class QTcpSocket;
class QTcpServer;

class Transport : public QObject
{
    Q_OBJECT

public:
    virtual bool write(QByteArray ba) = 0;
    virtual void abort()
    {}

    QString socketString(QTcpSocket* tcpSocket);

signals:
    void readyRead(QByteArray*);
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
};


class TcpClientTransport : public Transport
{
    Q_OBJECT

    quint16 port;
    QHostAddress hostAddress;
    int writeLimit;

    QSharedPointer<QTcpSocket> tcpSocket;
    QByteArray buffer;
    int reconnectTimerId;

public:
    explicit TcpClientTransport(quint16 port, QHostAddress hostAddress, int writeLimit = 1024*1024);
    bool write(QByteArray ba);
    void abort();

public slots:
    void onNewConnection();
    void onDisconnected();

    void onError(QAbstractSocket::SocketError socketError);
    void timerEvent(QTimerEvent *);
    void onSocketRead();
};

class TcpServerTransport : public Transport
{
    Q_OBJECT

    int writeLimit;
    QSharedPointer<QTcpServer> tcpServer;
    QList<QSharedPointer<QTcpSocket>> socketList;
    QMap<QSharedPointer<QTcpSocket>, QByteArray> dataMap;
public:
    explicit TcpServerTransport(quint16 port, QHostAddress hostAddress = QHostAddress::Any, int writeLimit = 1024*1024);
    bool write(QByteArray ba);

public slots:
    void onNewConnection();
    void onDisconnected();
    void onSocketRead();
};

#endif // TCPTRANSPORT_H
