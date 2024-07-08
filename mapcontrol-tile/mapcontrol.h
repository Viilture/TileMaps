#ifndef MAPCONTROL_H
#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QSetIterator>
#include <QPainterPath>
#include <QConicalGradient>
#include <set>
#include <memory>

#include "MapTileSource.h"
#include "MapGraphics_global.h"
#include "guts/MapTileGraphicsObject.h"
#include "guts/PrivateQGraphicsInfoSource.h"
#include "tileSources/GridTileSource.h"
#include "tileSources/OSMTileSource.h"
#include "tileSources/CompositeTileSource.h"
#include "scenecontrol.h"

#include "settingsBR.h"
#include "mapline.h"
#include "mapradar.h"
#include "mapmeasure.h"
#include "mapcamera.h"
#include "maptarget.h"
#include "targetcontrol.h"

class sceneControl;
class MapLine;
class mapMeasure;
class mapRadar;
class mapCamera;
class mapTarget;
class TargetControl;
class SettingsBR;

/** @class Класс управления тайловыми картами */
enum ZoomMode
{
    CenterZoom,
    MouseZoom
};

class MapControl: public QObject
{
    Q_OBJECT

public:
    explicit MapControl(sceneControl* newscenControl);
    virtual ~MapControl();

    void centerOn(const QPointF& pos);
    void centerOn(qreal longitude, qreal latitude);
    //void centerOn(const MapGraphicsObject * item);

    void setTileSource(QSharedPointer<MapTileSource>  tSource);
    void setZoomLevel(quint8 nZoom, ZoomMode zMode);

    quint8 zoomLevel() const;
    /** @brief Преобразование широты и долготы в координаты сцены */
    QPointF ll2qgs(const QPointF& ll, quint8 zoomLevel);
    QPointF ll2qgs(const QPointF& ll);
    /** @brief Преобразование координат сцены в широту и долготу */
    QPointF qgs2ll(const QPointF& ll, quint8 zoomLevel);
    QPointF qgs2ll(const QPointF& ll);

    /** @brief Создаем новый обьект линии на карте */
    MapLine* createLine(QPointF point1 = QPointF(30, 30), QPointF point2 = QPointF(70, 70));

    /** @brief Создаем новый обьект Радара на карте */
    QSharedPointer<mapRadar> createRadar(QPointF centerP = QPointF(30, 30));

    /** @brief Удалить номер Радара */
    void removeRadar(QSharedPointer<mapRadar> mapRadar);

    /** @brief Создаем новый обьект камеры на карте */
    QSharedPointer<mapCamera> createCamera(QPointF centerP = QPointF(30, 30));

    /** @brief Удалить номер камеры */
    void removeCamera(QSharedPointer<mapCamera> mapCamera);

    /** @brief Создаем новый обьект Линейки на карте */
    QSharedPointer<mapMeasure> createMeasure();
    /** @brief Удалить Линейку */
    void removeMeasure(QSharedPointer<mapMeasure> mapMeasure);

    /** @brief Создаем новый обьект отображения целей на карте */
    QSharedPointer<mapTarget> createTarget();

    /** @brief Удаляем обьект отображения целей на карте */
    void removeTarget(QSharedPointer<mapTarget> mapTarg);

    /** @brief Создаем новый обьект отображения целей на карте */
    QSharedPointer<TargetControl> createTargetControl();

    /** @brief Удаляем обьект отображения целей на карте */
    void removeTargetControl(QSharedPointer<TargetControl> mapTarg);

    /** \brief Меняем путь до фалов карты */
    QString getPathMaps();

    void doTileLayout();


    QSharedPointer<MapTileSource> tileSource() const;

private:
    /** @brief Cсылка на класс управления сценой */
    sceneControl* scenControl;
    bool  updateAll = false;

    QSharedPointer<SettingsBR> set;

    quint8 _zoomLevel = 3;              //уровень масштабирования по умолчанию на 3

    QSharedPointer<MapTileSource> _tileSource;
    QSet<MapTileGraphicsObject *> _tileObjects;
    QSet<QSharedPointer<MapLine>> mapLines;
    QSet<QSharedPointer<mapRadar>> mapRadars;
    QSet<QSharedPointer<mapCamera>> mapCameras;
    QSet<QSharedPointer<mapMeasure>> mapMeasures;
    QSet<QSharedPointer<mapTarget>> mapTargets;
    QSet<QSharedPointer<TargetControl>> mapTargContrs;

    void init();

    /** @brief Масштабирование обновилось */
    void zoomUpdates();

protected:
    void resetQGSSceneSize();

Q_SIGNALS:
    void zoomUpdate();
    void setTypeMap(typeMaps newType = typeMaps::googleMaps);

public Q_SLOTS:
    /*! \brief Устанавлиаем тип карты */
    void setMapType(typeMaps newType = typeMaps::googleMaps);

    /*! \brief Меняем путь до фалов карты */
    void setPathMaps(QString path);

};

inline size_t qHash(const QPointF& key)
{
    const QString temp = QString::number(key.x()) % "," % QString::number(key.y());
    return qHash(temp);
}

#endif // MAPCONTROL_H
