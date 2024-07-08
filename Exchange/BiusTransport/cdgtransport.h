#ifndef CDGTRANSPORT_H
#define CDGTRANSPORT_H

#include <QObject>
#include <QHostAddress>
#include <QSharedPointer>

class Transport;

class Cdg : public QObject
{
    Q_OBJECT

    static const int headerSize = sizeof(int);
    QList<QByteArray> cdgList;
protected:
    QSharedPointer<Transport> transport;
public:
    explicit Cdg(QSharedPointer<Transport> newTransport);
    bool writeCdg(QByteArray cdg);
    bool readCdg(QByteArray* cdg);

    virtual void abort() = 0;

    bool isCdgAvailable()
    {
        return !cdgList.isEmpty();
    }

signals:
    void readyRead();
private slots:
    void onReadyRead(QByteArray *);

};

class CdgServer : public Cdg
{
    Q_OBJECT

public:
    explicit CdgServer(quint16 port, QHostAddress hostAddress = QHostAddress::Any);
    void abort() override;
signals:
    void connected();
    void disconnected();

};

class CdgClient : public Cdg
{
    Q_OBJECT

public:
    explicit CdgClient(quint16 port, QString hostAddress);
    void abort() override;
signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);

};

#endif // CDGTRANSPORT_H
