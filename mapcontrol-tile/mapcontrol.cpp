#include "mapcontrol.h"

#define Barier2INI "/Barier2INI"
#define IntervalUpdateMap 250

class managerSignals;

MapControl::MapControl(sceneControl *newscenControl):scenControl(newscenControl)
{
    init();
}

MapControl::~MapControl()
{

}

void MapControl::init()
{
    qRegisterMetaType<typeMaps>("typeMaps");

    //Настройте некоторые источники плитки
    QSharedPointer<MapTileSource> osmTiles(new OSMTileSource(), &QObject::deleteLater);
    QSharedPointer<CompositeTileSource> composite(new CompositeTileSource(), &QObject::deleteLater);
    composite->addSourceBottom(osmTiles);
    setTileSource(composite);

    connect(this, &MapControl::setTypeMap, composite.data(), &CompositeTileSource::setMapType);

    static QTimer* timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=](){doTileLayout();});
    timer->start(IntervalUpdateMap);

    doTileLayout();

    //Восстанавливаем тип карты и координаты из ini файла
    set = QSharedPointer<SettingsBR>::create();

    QString typeMap = set->value(QStringLiteral("Map/typeMap"), "googleGEOMaps").toString();

    if(typeMap == QLatin1String("googleGEOMaps"))     setMapType(typeMaps::googleGEOMaps);
    else if(typeMap == QLatin1String("osmMaps"))      setMapType(typeMaps::osmMaps);
    else if(typeMap == QLatin1String("googleMaps"))   setMapType(typeMaps::googleMaps);
    else if(typeMap == QLatin1String("statmen"))      setMapType(typeMaps::statmen);
    else if(typeMap == QLatin1String("openToppoMap")) setMapType(typeMaps::openToppoMap);

    //Сохраняем все в Ini файл
    quint8 zoomV = set->value(QStringLiteral("Map/zoom"), 6).toUInt();
    setZoomLevel(zoomV, ZoomMode::MouseZoom);

    double longi = set->value(QStringLiteral("Map/long"), -77.0561).toDouble();
    double lat   = set->value(QStringLiteral("Map/lat"), 38.8709).toDouble();

    centerOn(longi, lat);

    static auto t = createTarget();

    //static auto trg = createTargets();
   // trg->setCenter(QPointF(-77.0561, 38.8709));

//    static QTimer time;
//    static std::vector<Target> nT;
//    nT.push_back(Target{1000, 45});
//    trg->setTargets(nT);

}

/** @brief Масштабирование обновилось */
void MapControl::zoomUpdates()
{
    Q_EMIT zoomUpdate();

    QTime time = QTime::currentTime();

    //Перебираем все линии, если линия не видна, удаялем её из массива, потом сразу же SharedPointer вызовет деструктор
    for(auto &it:qAsConst(mapLines))
    {
        if(!it.isNull())
            if(it->isVisible())
                it->updateGraph();
            else
            {
                scenControl->scene()->removeItem(it.data());
                mapLines.remove(it);
            }
        else
            mapLines.remove(it);
    }

    for(auto &it:qAsConst(mapRadars))
    {
        if(!it.isNull())
        {
            it->updateGraph();
        }
        else
            mapRadars.remove(it);
    }

    for(auto &it:qAsConst(mapCameras))
    {
        if(!it.isNull())
        {
            it->updateGraph();
        }
        else
            mapCameras.remove(it);
    }

    for(auto &it:qAsConst(mapMeasures))
    {
        if(it != nullptr)
        {
            it->updateGraph();
        }
        else
            mapMeasures.remove(it);
    }
    for(auto &it:qAsConst(mapTargets))
    {
        if(it != nullptr)
        {
            it->updateGraph();
        }
        else
            mapTargets.remove(it);
    }

    doTileLayout();
}

void MapControl::setTileSource(QSharedPointer<MapTileSource> tSource)
{
    _tileSource = tSource;

    if (!_tileSource.isNull())
    {
        //Создайте поток только для источника плитки
        QThread * tileSourceThread = new QThread();
        tileSourceThread->start();
        _tileSource->moveToThread(tileSourceThread);

        //Настройте его так, чтобы поток уничтожался, когда источник тайла!
        connect(_tileSource.data(),
                &QObject::destroyed,
                tileSourceThread,
                &QThread::quit);
        connect(tileSourceThread,
                &QThread::finished,
                tileSourceThread,
                &QObject::deleteLater);

    }

    //Обновите наши отображения плитки (если есть) о новом источнике плитки.
    Q_FOREACH(MapTileGraphicsObject * tileObject, _tileObjects)
        tileObject->setTileSource(tSource);
}

void MapControl::setZoomLevel(quint8 nZoom, ZoomMode zMode)
{
    if (_tileSource.isNull())
        return;

    //Этот материал предназначен для обработки повторного центрирования при увеличении/уменьшении масштаба.
    const QPointF  centerGeoPos =  scenControl->mapToScene(QPoint(scenControl->width()/2, scenControl->height()/2));
    QPointF mousePoint =  scenControl->mapToScene(scenControl->mapFromGlobal(QCursor::pos()));
    QRectF sceneRect =  scenControl->getScene()->sceneRect();
    const float xRatio = mousePoint.x() / sceneRect.width();
    const float yRatio = mousePoint.y() / sceneRect.height();
    const QPointF centerPos = scenControl->mapToScene(QPoint(scenControl->width()/2, scenControl->height()/2));
    const QPointF offset = mousePoint - centerPos;

    //Изменить уровень масштабирования
    nZoom = qMin(_tileSource->maxZoomLevel(),qMax(_tileSource->minZoomLevel(),nZoom));

    if (nZoom == _zoomLevel)
        return;

    _zoomLevel = nZoom;

    //Временно отключите отображение всех плиток. Они будут отображаться правильно, когда таймер тикает
    Q_FOREACH(MapTileGraphicsObject * tileObject, _tileObjects)
        tileObject->setVisible(false);

    //Убедитесь, что QGraphicsScene имеет правильный размер.
    this->resetQGSSceneSize();

    //Повторное центрирование вида там, где мы этого хотим
    sceneRect = scenControl->getScene()->sceneRect();
    mousePoint = QPointF(sceneRect.width()*xRatio,
                         sceneRect.height()*yRatio) - offset;

    if (zMode == MouseZoom)
        centerOn(mousePoint);
    else
        this->centerOn(centerGeoPos);

    zoomUpdates();

    //Сохраняем все в Ini файл
    if(!set.isNull())
        set->setValue(QStringLiteral("Map/zoom"), nZoom);
}

void MapControl::doTileLayout()
{
    static QPointF oldcenterPointQGS = QPointF(0,0);

    /*Первая часть кода вычисляет центральную точку и многоугольник области просмотра в координатах QGraphicsScene.
     Для этого используется метод mapToScene(), который преобразует координаты QGraphicsView в координаты QGraphicsScene. */

    const QPointF centerPointQGS = scenControl->mapToScene(scenControl->width()/2.0, scenControl->height()/2.0);

    // createRadar(centerPointQGS);

    //Время прошедшее после последнего обновления тайлов
    static QTime oldTimer  = QTime::currentTime();


    if(updateAll == false)
    {
        //Если обновление не нужно, выходим
        if(oldcenterPointQGS == centerPointQGS) return;

        if((QTime::currentTime().msecsSinceStartOfDay() - oldTimer.msecsSinceStartOfDay()) < 150) return;
    }

    oldTimer = QTime::currentTime();
    oldcenterPointQGS = centerPointQGS;

    //Вычисление многоугольника области просмотра
    QPolygon viewportPolygonQGV;
    viewportPolygonQGV << QPoint(0,0) << QPoint(0,  scenControl->height()) << QPoint(scenControl->width(),  scenControl->height()) << QPoint(scenControl->width(),0);

    const QPolygonF viewportPolygonQGS = scenControl->mapToScene(viewportPolygonQGV);
    //Вычисление ограничивающего прямоугольника
    const QRectF boundingRect = viewportPolygonQGS.boundingRect();

    /* Далее создается прямоугольник exaggeratedBoundingRect, который является немного больше,
      * чем ограничивающий прямоугольник boundingRect. Это делается для того, чтобы получить некоторый запас плиток,
      * которые еще не видны на экране, но могут быть видны при прокрутке карты. */

    QRectF exaggeratedBoundingRect = boundingRect;
    exaggeratedBoundingRect.setSize(boundingRect.size()*2.0);
    exaggeratedBoundingRect.moveCenter(boundingRect.center());

    //Мы пометим плитки, которые не отображаются, как бесплатные, чтобы мы могли их использовать.
    QQueue<MapTileGraphicsObject *> freeTiles;

    QSet<QPointF> placesWhereTilesAre;

    /* Далее идет цикл по всем плиткам, которые уже есть на QGraphicsScene.
      * Если плитка не видна или не попадает в увеличенный ограничивающий прямоугольник,
      * то она помечается как бесплатная и становится невидимой. Если же плитка видна, то ее координата добавляется
      * в множество placesWhereTilesAre.
      * */
    if(updateAll == false)
    {
        Q_FOREACH(MapTileGraphicsObject * tileObject, _tileObjects)
        {
            if (!tileObject->isVisible() || !exaggeratedBoundingRect.contains(tileObject->pos()))
            {
                freeTiles.enqueue(tileObject);
                tileObject->setVisible(false);
            }
            else
                placesWhereTilesAre.insert(tileObject->pos());
        }
    }

    //Если нужна полная перезагрузка, очищаем все тайлы
    else{

        Q_FOREACH(MapTileGraphicsObject * tileObject, _tileObjects)
        {
            _tileObjects.remove(tileObject);
            scenControl->getScene()->removeItem(tileObject);
            delete tileObject;

        }
    }

    /* Далее вычисляются необходимые параметры для цикла, который создаст новые плитки.
      * tileSize - размер одной плитки в пикселях, tilesPerRow и tilesPerCol - количество плиток в строке
      * и столбце на данном уровне масштабирования, perSide - количество плиток в каждом направлении от
      * центральной точки, xc и yc - координаты верхнего левого угла прямоугольника,
      * в который вписывается увеличенный ограничивающий прямоугольник, xMax и yMax - координаты правого нижнего угла этого прямоугольника.
    */
    const quint16 tileSize = _tileSource->tileSize();
    const quint32 tilesPerRow = sqrt((long double)_tileSource->tilesOnZoomLevel(this->zoomLevel()));
    const quint32 tilesPerCol = tilesPerRow;

    const qint32 perSide = qMax(boundingRect.width()/tileSize,
                                boundingRect.height()/tileSize) + 3;
    const qint32 xc = qMax((qint32)0,
                           (qint32)(centerPointQGS.x() / tileSize) - perSide/2);
    const qint32 yc = qMax((qint32)0,
                           (qint32)(centerPointQGS.y() / tileSize) - perSide/2);
    const qint32 xMax = qMin((qint32)tilesPerRow,
                             xc + perSide);
    const qint32 yMax = qMin(yc + perSide,
                             (qint32)tilesPerCol);

    for (qint32 x = xc; x < xMax; x++)
    {
        for (qint32 y = yc; y < yMax; y++)
        {
            const QPointF scenePos(x*tileSize + tileSize/2,
                                   y*tileSize + tileSize/2);


            bool tileIsThere = false;
            if (placesWhereTilesAre.contains(scenePos))
                tileIsThere = true;

            if (tileIsThere)
                continue;

            //На всякий случай, если у нас заканчиваются бесплатные плитки, добавьте одну
            if (freeTiles.isEmpty())
            {
                MapTileGraphicsObject * tileObject = new MapTileGraphicsObject(tileSize);
                tileObject->setTileSource(_tileSource);
                _tileObjects.insert(tileObject);
                scenControl->getScene()->addItem(tileObject);
                freeTiles.enqueue(tileObject);
            }
            //Получите первую бесплатную плитку и заставьте ее делать свое дело
            MapTileGraphicsObject * tileObject = freeTiles.dequeue();
            if (tileObject->pos() != scenePos)
                tileObject->setPos(scenePos);
            if (tileObject->isVisible() != true)
                tileObject->setVisible(true);
            tileObject->setTile(x,y,this->zoomLevel());
        }
    }

    //Если у нас осталось много свободных плиток, удалите часть из них
    while (freeTiles.size() > 2)
    {
        MapTileGraphicsObject * tileObject = freeTiles.dequeue();
        _tileObjects.remove(tileObject);
        scenControl->getScene()->removeItem(tileObject);
        delete tileObject;
    }

    updateAll = false;
}

QSharedPointer<MapTileSource> MapControl::tileSource() const
{
    return _tileSource;
}

/** @brief Создаем новый обьект Радара на карте
    @param QPointF centerP - Координата центра РЛУ в Широте и долготе WGS84 */
QSharedPointer<mapRadar> MapControl::createRadar(QPointF centerP)
{
    QSharedPointer<mapRadar> radar;

    if(scenControl->scene() == nullptr)
    {
        qDebug() << "ERROR MapControl::createRadar scenControl->scene() == nullptr";
        return radar;
    }

    radar = QSharedPointer<mapRadar>::create(this, centerP);

    mapRadars.insert(radar);
    scenControl->scene()->addItem(radar.data());

    return radar;
}

/** @brief Удалить номер Радара */
void MapControl::removeRadar(QSharedPointer<mapRadar> mapRadar)
{
    if(mapRadar.isNull()) return;

    mapRadars.remove(mapRadar);

    scenControl->scene()->removeItem(mapRadar.data());
}

/** @brief Создаем новый обьект камеры на карте */
QSharedPointer<mapCamera> MapControl::createCamera(QPointF centerP)
{
    QSharedPointer<mapCamera> camera;

    if(scenControl->scene() == nullptr)
    {
        qDebug() << "ERROR MapControl::createRadar scenControl->scene() == nullptr";
        return camera;
    }

    camera = QSharedPointer<mapCamera>::create(this, centerP);

    mapCameras.insert(camera);
    scenControl->scene()->addItem(camera.data());

    return camera;
}

/** @brief Удалить номер камеры */
void MapControl::removeCamera(QSharedPointer<mapCamera> mapCamera)
{
    if(mapCamera.isNull()) return;

    mapCameras.remove(mapCamera);

    scenControl->scene()->removeItem(mapCamera.data());
}

/** @brief Создаем новый обьект Линейки на карте */
QSharedPointer<mapMeasure> MapControl::createMeasure()
{

    QSharedPointer<mapMeasure> mapMeas;

    if(scenControl->scene() == nullptr)
    {
        qDebug() << "ERROR MapControl::createMeasure scenControl->scene() == nullptr";
        return mapMeas;
    }

    mapMeas = QSharedPointer<mapMeasure>::create(this);

    mapMeasures.insert(mapMeas);
    scenControl->scene()->addItem(mapMeas.get());

    return mapMeas;

}

/** @brief Удалить Линейку */
void MapControl::removeMeasure(QSharedPointer<mapMeasure> mapMeasure)
{
    if(mapMeasure == nullptr) return;

    mapMeasures.remove(mapMeasure);

    scenControl->scene()->removeItem(mapMeasure.get());
}

/** @brief Создаем новый обьект отображения целей на карте */
QSharedPointer<mapTarget> MapControl::createTarget()
{
    QSharedPointer<mapTarget> mapTarget;

    if(scenControl->scene() == nullptr)
    {
        qDebug() << "ERROR MapControl::createTargets scenControl->scene() == nullptr";
        return mapTarget;
    }

    mapTarget = QSharedPointer<class mapTarget>::create(this);

    mapTargets.insert(mapTarget);
    scenControl->scene()->addItem(mapTarget.get());
    return mapTarget;
}

/** @brief Удаляем обьект отображения целей на карте */
void MapControl::removeTarget(QSharedPointer<mapTarget> mapTarg)
{
    if(mapTarg == nullptr) return;

    mapTargets.remove(mapTarg);

    scenControl->scene()->removeItem(mapTarg.get());
}

QSharedPointer<TargetControl> MapControl::createTargetControl()
{
    QSharedPointer<TargetControl> targControl;

    if(scenControl->scene() == nullptr)
    {
        qDebug() << "ERROR MapControl::ccreateTargetControl scenControl->scene() == nullptr";
        return targControl;
    }

    targControl = QSharedPointer<TargetControl>::create(this);

    mapTargContrs.insert(targControl);

    return targControl;
}

void MapControl::removeTargetControl(QSharedPointer<TargetControl> trgControl)
{
    if(trgControl == nullptr) return;

  //  trgControl->removeAllT
    mapTargContrs.remove(trgControl);
}

void MapControl::centerOn(const QPointF &pos)
{
    if (_tileSource.isNull())
        return;

    scenControl->centerOn(pos);

}

void MapControl::centerOn(qreal longitude, qreal latitude)
{
    //Найдите координату QGraphicsScene позиции, а затем сообщите дочернему представлению, чтобы центрировать ее.
    this->centerOn(_tileSource->ll2qgs(QPointF(longitude, latitude),this->zoomLevel()));

    //Сохраняем все в Ini файл
    if(!set.isNull())
    {
        set->setValue(QStringLiteral("Map/long"), longitude);
        set->setValue(QStringLiteral("Map/lat"), latitude);
    }
}

//защищенный
void MapControl::resetQGSSceneSize()
{
    if (_tileSource.isNull())
        return;

    //Убедитесь, что размер нашей QGraphicsScene правильный.
    const quint64 dimension = sqrt((long double)_tileSource->tilesOnZoomLevel(this->zoomLevel()))*_tileSource->tileSize();
    if (scenControl->getScene()->sceneRect().width() != dimension)
        scenControl->getScene()->setSceneRect(0,0,dimension,dimension);
}

void MapControl::setMapType(typeMaps newType)
{
    if(_tileSource.isNull()) return;

    Q_EMIT setTypeMap(newType);

    updateAll = true;

    //И сохраняем все

    if(!set.isNull())
        switch (newType)
        {
        case(typeMaps::googleGEOMaps) :{set->setValue(QStringLiteral("Map/typeMap"), "googleGEOMaps"); break;}
        case(typeMaps::osmMaps)       :{set->setValue(QStringLiteral("Map/typeMap"), "osmMaps");       break;}
        case(typeMaps::googleMaps)    :{set->setValue(QStringLiteral("Map/typeMap"), "googleMaps");    break;}
        case(typeMaps::statmen)       :{set->setValue(QStringLiteral("Map/typeMap"), "statmen");       break;}
        case(typeMaps::openToppoMap)  :{set->setValue(QStringLiteral("Map/typeMap"), "openToppoMap");  break;}
        default: break;

        }
}

/*! \brief Меняем путь до фалов карты */
void MapControl::setPathMaps(QString path)
{
    if(_tileSource.isNull() || path.isEmpty()) return;

    _tileSource->setMAPGRAPHICS_CACHE_FOLDER_NAME(path);
}

QString MapControl::getPathMaps()
{
    if(_tileSource.isNull()) return QLatin1String("");

    return _tileSource->getMAPGRAPHICS_CACHE_FOLDER_NAME();
}

quint8 MapControl::zoomLevel() const
{
    return _zoomLevel;
}

/** @brief Преобразование широты и долготы в координаты сцены */
QPointF MapControl::ll2qgs(const QPointF &ll, quint8 zoomLevel)
{
    return _tileSource->ll2qgs(std::move(ll), std::move(zoomLevel));
}

QPointF MapControl::ll2qgs(const QPointF &ll)
{
     return _tileSource->ll2qgs(std::move(ll), zoomLevel());
}

/** @brief Преобразование координат сцены в широту и долготу */
QPointF MapControl::qgs2ll(const QPointF &ll, quint8 zoomLevel)
{
    return _tileSource->qgs2ll(std::move(ll), std::move(zoomLevel));
}

QPointF MapControl::qgs2ll(const QPointF &ll)
{
    return _tileSource->qgs2ll(std::move(ll), zoomLevel());
}

/** @brief Создаем новый обьект линии на карте */
MapLine* MapControl::createLine(QPointF point1, QPointF point2)
{
    if(scenControl->scene() == nullptr) return nullptr;

    QSharedPointer<MapLine> line = QSharedPointer<MapLine>::create(point1, point2, this);

    mapLines.insert(line);
    scenControl->scene()->addItem(line.data());

    return line.data();
}
