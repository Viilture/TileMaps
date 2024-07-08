#ifndef OSMTILESOURCE_H
#define OSMTILESOURCE_H

#include "MapTileSource.h"
#include "MapGraphics_global.h"
#include <QSet>
#include <QSettings>
#include <QHash>
#include <QCoreApplication>

//Предварительное объявление, чтобы проекты, которые импортируют нас как библиотеку, не обязательно использовали QT += network
class QNetworkReply;

class OSMTileSource : public MapTileSource
{
    Q_OBJECT
public:


public:
    explicit OSMTileSource(QObject *parent = nullptr);
    virtual ~OSMTileSource();

    virtual QPointF ll2qgs(const QPointF& ll, quint8 zoomLevel) const;

    virtual QPointF qgs2ll(const QPointF& qgs, quint8 zoomLevel) const;

    virtual quint64 tilesOnZoomLevel(quint8 zoomLevel) const;

    virtual quint16 tileSize() const;

    virtual quint8 minZoomLevel(QPointF ll);

    virtual quint8 maxZoomLevel(QPointF ll);

    virtual QString name() const;

    virtual QString tileFileExtension() const;

protected:
    virtual void fetchTile(quint32 x,
                           quint32 y,
                           quint8 z);

private:


    //Set используется для обеспечения того, чтобы плитка с определенным идентификатором кэша не запрашивалась дважды
    QSet<QString> _pendingRequests;


    //Хэш, используемый для отслеживания того, что кэшируется вместе с каким ответом
    QHash<QNetworkReply *, QString> _pendingReplies;

Q_SIGNALS:

public Q_SLOTS:
    /*! \brief Устанавлиаем тип карты */
    void setMapType(typeMaps newType = typeMaps::googleMaps) override;

private Q_SLOTS:
    void handleNetworkRequestFinished();



};

#endif // OSMTILESOURCE_H
