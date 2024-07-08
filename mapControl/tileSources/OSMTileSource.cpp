#include "OSMTileSource.h"

#include <QtMath>

#include "guts/MapGraphicsNetwork.h"

#include <cmath>
#include <QPainter>
#include <QStringBuilder>
#include <QtDebug>
#include <QNetworkReply>

const qreal PI = 3.14159265358979323846;
const qreal deg2rad = PI / 180.0;
const qreal rad2deg = 180.0 / PI;
#define Barier2INI "/Barier2INI"

OSMTileSource::OSMTileSource(QObject *parent) :
    MapTileSource(parent)
{
    this->setCacheMode(MapTileSource::DiskAndMemCaching);
}

OSMTileSource::~OSMTileSource()
{

}

QPointF OSMTileSource::ll2qgs(const QPointF &ll, quint8 zoomLevel) const
{
    const qreal tilesOnOneEdge = pow(2.0,zoomLevel);
    const quint16 tileSize = this->tileSize();
    qreal x = (ll.x()+180) * (tilesOnOneEdge*tileSize)/360; // coord to pixel!
    qreal y = (1-(log(tan(PI/4+(ll.y()*deg2rad)/2)) /PI)) /2  * (tilesOnOneEdge*tileSize);

    return QPointF(int(x), int(y));
}

QPointF OSMTileSource::qgs2ll(const QPointF &qgs, quint8 zoomLevel) const
{
    const qreal tilesOnOneEdge = pow(2.0,zoomLevel);
    const quint16 tileSize = this->tileSize();
    qreal longitude = (qgs.x()*(360/(tilesOnOneEdge*tileSize)))-180;
    qreal latitude = rad2deg*(atan(sinh((1-qgs.y()*(2/(tilesOnOneEdge*tileSize)))*PI)));

    return QPointF(longitude, latitude);
}

quint64 OSMTileSource::tilesOnZoomLevel(quint8 zoomLevel) const
{
    return pow(4.0,zoomLevel);
}

quint16 OSMTileSource::tileSize() const
{
    return 256;
}

quint8 OSMTileSource::minZoomLevel(QPointF ll)
{
    Q_UNUSED(ll)
    return 0;
}

quint8 OSMTileSource::maxZoomLevel(QPointF ll)
{
    Q_UNUSED(ll)
    switch (typeMap) {
    case typeMaps::osmMaps:
        return 18;
        break;
    case typeMaps::googleGEOMaps:
        return 20;
        break;
    case typeMaps::googleMaps:
        return 20;
        break;
    case typeMaps::statmen:
        return 18;
        break;
    case typeMaps::openToppoMap:
        return 18;
        break;

    default:
        break;

    }
    return 18;
}

QString OSMTileSource::name() const
{
    switch (typeMap) {
    case typeMaps::osmMaps:
        return QStringLiteral("OpenStreetMap Tiles");
        break;
    case typeMaps::googleGEOMaps:
        return QStringLiteral("GoogleGEOMap Tiles");
        break;
    case typeMaps::googleMaps:
        return QStringLiteral("GoogleMap Tiles");
        break;
    case typeMaps::statmen:
        return QStringLiteral("StatmenMap Tiles");
        break;
    case typeMaps::openToppoMap:
        return QStringLiteral("User Tiles");
        break;

    default:
        break;

    }
    return QString();
}

QString OSMTileSource::tileFileExtension() const
{
    if (typeMap == osmMaps)
        return QStringLiteral("png");
    else
        return QStringLiteral("png");
}

//protected
void OSMTileSource::fetchTile(quint32 x, quint32 y, quint8 z)
{
    MapGraphicsNetwork * network = MapGraphicsNetwork::getInstance();

    QString host;
    QString url;

    QSettings set(QCoreApplication::applicationDirPath() + Barier2INI, QSettings::IniFormat);

//    set.setValue("Map/httpOSM",      "http://b.tile.openstreetmap.org");
//    set.setValue("Map/httpGooglGeo", "http://khms0.google.com");
//    set.setValue("Map/httpGoogl",    "http://mt.google.com/vt/lyrs=m");
//    set.setValue("Map/httpStat",     "http://stamen-tiles.a.ssl.fastly.net/terrain/");
//    set.setValue("Map/httpUserMap",  "https://core-sat.maps.yandex.net/tiles?l=sat&v=3.1075.0&x=");

//    set.setValue("Map/urlOSM",      "/%3/%1/%2.png");
//    set.setValue("Map/urlGooglGeo", "/kh/v=947?x=%1&y=%2&z=%3");
//    set.setValue("Map/urlGoogl",    "&x=%1&y=%2&z=%3");
//    set.setValue("Map/urlStat",     "%3/%1/%2.png");
//    set.setValue("Map/urlUserMap",  "%1&y=%2&z=%3&scale=1&lang=ru_RU");

    static QString httpOSM      =  set.value(QStringLiteral("Map/httpOSM"),      "http://b.tile.openstreetmap.org")                           .toString();
    static QString httpGooglGeo =  set.value(QStringLiteral("Map/httpGooglGeo"), "http://khms0.google.com")                                   .toString();
    static QString httpGoogl    =  set.value(QStringLiteral("Map/httpGoogl"),    "http://mt.google.com/vt/lyrs=m")                            .toString();
    static QString httpStat     =  set.value(QStringLiteral("Map/httpStat"),     "http://stamen-tiles.a.ssl.fastly.net/terrain/")             .toString();
    static QString httpUserMap  =  set.value(QStringLiteral("Map/httpUserMap"),  "https://core-sat.maps.yandex.net/tiles?l=sat&v=3.1075.0&x=").toString();

    static QString urlOSM       = set.value(QStringLiteral("Map/urlOSM"),      "/%3/%1/%2.png")                  .toString();
    static QString urlGooglGeo  = set.value(QStringLiteral("Map/urlGooglGeo"), "/kh/v=947?x=%1&y=%2&z=%3")       .toString();
    static QString urlGoogl     = set.value(QStringLiteral("Map/urlGoogl"),    "&x=%1&y=%2&z=%3")                .toString();
    static QString urlStat      = set.value(QStringLiteral("Map/urlStat"),     "%3/%1/%2.png")                   .toString();
    static QString urlUserMap   = set.value(QStringLiteral("Map/urlUserMap"),  "%1&y=%2&z=%3&scale=1&lang=ru_RU").toString();


    switch (typeMap) {
    case typeMaps::osmMaps:
    {
        host = httpOSM;
        url  = urlOSM;
        break;
    }
    case typeMaps::googleGEOMaps:
    {
        host = httpGooglGeo;
        url  = urlGooglGeo;

        break;
    }
    case typeMaps::googleMaps:
    {
        host = httpGoogl;
        url  = urlGoogl;

        break;
    }
    case typeMaps::statmen:
    {
        host = httpStat;
        url  = urlStat;


        break;
    }
    case typeMaps::openToppoMap:
    {
        host = httpUserMap;
        url  = urlUserMap;

        break;
    }


    default:
    {
        break;
    }

    }

    //Используйте уникальный идентификатор кэша, чтобы узнать, была ли эта плитка уже запрошена
    const QString cacheID = this->createCacheID(x,y,z);
    if (_pendingRequests.contains(cacheID))
        return;
    _pendingRequests.insert(cacheID);

    //Build the request
    const QString fetchURL = url.arg(QString::number(x),
                                     QString::number(y),
                                     QString::number(z));
    qDebug() << host + fetchURL;

    QNetworkRequest request(QUrl(host + fetchURL));


    //Отправьте запрос и настройте сигнал, чтобы мы были уведомлены, когда он завершится
    QNetworkReply * reply = network->get(request);
    _pendingReplies.insert(reply,cacheID);

    connect(reply,
            &QNetworkReply::finished,
            this,
            &OSMTileSource::handleNetworkRequestFinished);
}

void OSMTileSource::setMapType(typeMaps newType)
{
    typeMap = newType;
}

//private slot
void OSMTileSource::handleNetworkRequestFinished()
{
    QObject * sender = QObject::sender();
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender);

    if (reply == 0)
    {
        qWarning() << "QNetworkReply cast failure";
        return;
    }

    /* Мы можем сделать это здесь и использовать reply позже в функции, потому что ответ
       не будет удален до тех пор, пока выполнение не вернется в цикл обработки событий. */
    reply->deleteLater();

    if (!_pendingReplies.contains(reply))
    {
        qWarning() << "Unknown QNetworkReply";
        return;
    }


    ////получить кэшированный
    const QString cacheID = _pendingReplies.take(reply);
    _pendingRequests.remove(cacheID);


    //Если произошла сетевая ошибка, проигнорируйте ответ
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Network Error:" << reply->errorString();
        return;
    }


    //Преобразовать кэшированные данные обратно в координаты плитки x,y,z
    quint32 x,y,z;
    if (!MapTileSource::cacheID2xyz(cacheID,&x,&y,&z))
    {
        qWarning() << "Failed to convert cacheID" << cacheID << "back to xyz";
        return;
    }

    QByteArray bytes = reply->readAll();
    QImage * image = new QImage();

    if (!image->loadFromData(bytes))
    {
        delete image;
        qWarning() << "Failed to make QImage from network bytes";
        return;
    }


    //Выясните, как долго плитка должна быть кэширована
    QDateTime expireTime;
    if (reply->hasRawHeader("Cache-Control"))
    {

        //Мы поддерживаем директиву о максимальном возрасте только на данный момент
        const QByteArray cacheControl = reply->rawHeader("Cache-Control");
        QRegExp maxAgeFinder("max-age=(\\d+)");
        if (maxAgeFinder.indexIn(cacheControl) != -1)
        {
            bool ok = false;
            const qint64 delta = maxAgeFinder.cap(1).toULongLong(&ok);

            if (ok)
                expireTime = QDateTime::currentDateTimeUtc().addSecs(delta);
        }
    }

    //Уведомить клиента о получении плитки
    this->prepareNewlyReceivedTile(x,y,z, image, expireTime);
}
