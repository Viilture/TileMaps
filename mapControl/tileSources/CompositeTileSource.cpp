#include "CompositeTileSource.h"

#include <QtDebug>
#include <QPainter>
#include <QMutexLocker>
#include <QThread>
#include <QPointer>
#include <QTimer>

CompositeTileSource::CompositeTileSource(QObject *parent) :
    MapTileSource(parent)
{
    _globalMutex = new QMutex(QMutex::Recursive);
    this->setCacheMode(MapTileSource::NoCaching);
}

CompositeTileSource::~CompositeTileSource()
{
    _globalMutex->lock();

    qDebug() << this << "destructing";

    //Очистить все данные, относящиеся к отложенным плиткам
    this->clearPendingTiles();


    //Очистить исходники
    //Сначала мы находим все темы источников и помещаем их в список
    QList<QPointer<QThread> > tileSourceThreads;
    Q_FOREACH(QSharedPointer<MapTileSource> source, _childSources)
        tileSourceThreads.append(QPointer<QThread>(source->thread()));

    //Затем мы очищаем исходники
    _childSources.clear();

    int numThreads = tileSourceThreads.size();

    //Затем мы ждем, пока все эти потоки завершат работу
    for (int i = 0; i < numThreads; i++)
    {
        QPointer<QThread> thread = tileSourceThreads[i];
        if (!thread.isNull() && thread != this->thread())
            thread->wait(10000);
    }

    delete this->_globalMutex;
}

QPointF CompositeTileSource::ll2qgs(const QPointF &ll, quint8 zoomLevel) const
{
    QMutexLocker lock(_globalMutex);
    if (_childSources.isEmpty())
    {
        qWarning() << "Composite tile source is empty --- results undefined";
        return QPointF(0,0);
    }


    //Предположим, что все они одинаковы. Иначе делать нечего!
    return _childSources.at(0)->ll2qgs(ll,zoomLevel);
}

QPointF CompositeTileSource::qgs2ll(const QPointF &qgs, quint8 zoomLevel) const
{
    QMutexLocker lock(_globalMutex);
    if (_childSources.isEmpty())
    {
        qWarning() << "Composite tile source is empty --- results undefined";
        return QPointF(0,0);
    }


    //Предположим, что все они одинаковы. Иначе делать нечего!
    return _childSources.at(0)->qgs2ll(qgs,zoomLevel);
}

quint64 CompositeTileSource::tilesOnZoomLevel(quint8 zoomLevel) const
{
    QMutexLocker lock(_globalMutex);
    if (_childSources.isEmpty())
        return 1;
    else
        return _childSources.at(0)->tilesOnZoomLevel(zoomLevel);
}

quint16 CompositeTileSource::tileSize() const
{
    QMutexLocker lock(_globalMutex);
    if (_childSources.isEmpty())
        return 256;
    else
        return _childSources.at(0)->tileSize();
}

quint8 CompositeTileSource::minZoomLevel(QPointF ll)
{
    QMutexLocker lock(_globalMutex);

    //Возвращает самый высокий минимум
    quint8 highest = 0;

    Q_FOREACH(QSharedPointer<MapTileSource> source, _childSources)
    {
        quint8 current = source->minZoomLevel(ll);
        if (current > highest)
            highest = current;
    }
    return highest;
}

quint8 CompositeTileSource::maxZoomLevel(QPointF ll)
{
    QMutexLocker lock(_globalMutex);

    //Возвращает наименьший максимальный
    quint8 lowest = 50;

    Q_FOREACH(QSharedPointer<MapTileSource> source, _childSources)
    {
        quint8 current = source->maxZoomLevel(ll);
        if (current < lowest)
            lowest = current;
    }
    return lowest;
}

QString CompositeTileSource::name() const
{
    return QStringLiteral("Composite Tile Source");
}

QString CompositeTileSource::tileFileExtension() const
{
    return QStringLiteral(".jpg");
}

void CompositeTileSource::addSourceTop(QSharedPointer<MapTileSource> source, qreal opacity)
{
    QMutexLocker lock(_globalMutex);
    if (source.isNull())
        return;


    //Поместите дочерний элемент в его собственный поток
    this->doChildThreading(source);

    _childSources.insert(0, source);
    _childOpacities.insert(0,opacity);
    _childEnabledFlags.insert(0,true);

    connect(source.data(),
            &MapTileSource::tileRetrieved,
            this,
            &CompositeTileSource::handleTileRetrieved);

    Q_EMIT this->sourceAdded(0);
    Q_EMIT this->sourcesChanged();
    Q_EMIT this->allTilesInvalidated();
}

void CompositeTileSource::addSourceBottom(QSharedPointer<MapTileSource> source, qreal opacity)
{
    QMutexLocker lock(_globalMutex);
    if (source.isNull())
        return;


    //Поместите дочерний элемент в его собственный поток
    this->doChildThreading(source);

    _childSources.append(source);
    _childOpacities.append(opacity);
    _childEnabledFlags.append(true);

    connect(source.data(),
            &MapTileSource::tileRetrieved,
            this,
            &CompositeTileSource::handleTileRetrieved);

    Q_EMIT this->sourceAdded(_childSources.size()-1);
    Q_EMIT this->sourcesChanged();
    Q_EMIT this->allTilesInvalidated();
}

void CompositeTileSource::moveSource(int from, int to)
{
    if (from < 0 || to < 0)
        return;

    QMutexLocker lock(_globalMutex);

    int size = this->numSources();
    if (from >= size || to >= size)
        return;

    _childSources.move(from,to);
    _childOpacities.move(from,to);
    _childEnabledFlags.move(from,to);

    Q_EMIT this->sourcesReordered();
    Q_EMIT this->sourcesChanged();
    Q_EMIT this->allTilesInvalidated();
}

void CompositeTileSource::removeSource(int index)
{
    QMutexLocker lock(_globalMutex);
    if (index < 0 || index >= _childSources.size())
        return;

    _childSources.removeAt(index);
    _childOpacities.removeAt(index);
    _childEnabledFlags.removeAt(index);
    this->clearPendingTiles();

    Q_EMIT this->sourceRemoved(index);
    Q_EMIT this->sourcesChanged();
    Q_EMIT this->allTilesInvalidated();
}

int CompositeTileSource::numSources() const
{
    QMutexLocker lock(_globalMutex);
    return _childSources.size();
}

QSharedPointer<MapTileSource> CompositeTileSource::getSource(int index) const
{
    QMutexLocker lock(_globalMutex);
    if (index < 0 || index >= _childSources.size())
        return QSharedPointer<MapTileSource>();

    return _childSources[index];
}

qreal CompositeTileSource::getOpacity(int index) const
{
    QMutexLocker lock(_globalMutex);
    if (index < 0 || index >= _childSources.size())
        return 0.0;
    return _childOpacities[index];
}

void CompositeTileSource::setOpacity(int index, qreal opacity)
{
    opacity = qMin<qreal>(1.0,qMax<qreal>(0.0,opacity));

    QMutexLocker lock(_globalMutex);
    if (index < 0 || index >= _childSources.size())
        return;

    if (_childOpacities[index] == opacity)
        return;

    _childOpacities[index] = opacity;

    //подаем сигнал, чтобы сообщить всем наблюдающим за нами моделям, что мы изменились
    Q_EMIT this->sourcesChanged();
    Q_EMIT this->allTilesInvalidated();
}

bool CompositeTileSource::getEnabledFlag(int index) const
{
    QMutexLocker lock(_globalMutex);
    if (index < 0 || index >= _childSources.size())
        return 0.0;
    return _childEnabledFlags[index];
}

void CompositeTileSource::setEnabledFlag(int index, bool isEnabled)
{
    QMutexLocker lock(_globalMutex);
    if (index < 0 || index >= _childSources.size())
        return;

    if (_childEnabledFlags[index] == isEnabled)
        return;

    _childEnabledFlags[index] = isEnabled;

    Q_EMIT this->sourcesChanged();
    Q_EMIT this->allTilesInvalidated();
}

//protected
void CompositeTileSource::fetchTile(quint32 x, quint32 y, quint8 z)
{
    QMutexLocker lock(_globalMutex);
    //Если у нас нет дочерних источников, просто напечатайте сообщение об этом
    if (_childSources.isEmpty())
    {
        QImage * toRet = new QImage(this->tileSize(),
                                    this->tileSize(),
                                    QImage::Format_ARGB32_Premultiplied);
        QPainter painter;
        painter.begin(toRet);

        painter.fillRect(toRet->rect(),
                         Qt::white);
        painter.drawText(toRet->rect(),
                         QStringLiteral("Composite Source Empty"),
                         QTextOption(Qt::AlignCenter));
        painter.end();
        this->prepareNewlyReceivedTile(x,y,z,toRet);
        return;
    }

    //Выделите место в памяти для хранения фрагментов по мере их поступления, прежде чем мы их скомпонуем.
    ////Если у нас уже есть место, выделенное из предыдущего незавершенного запроса, очистите его и начните сначала
    QString cacheID = MapTileSource::createCacheID(x,y,z);
    if (_pendingTiles.contains(cacheID))
    {
        QMap<quint32, QImage *> * tiles = _pendingTiles.value(cacheID);
        Q_FOREACH(QImage * tile, *tiles)
            delete tile;
        tiles->clear();
    }

    //В противном случае создайте новое пространство
    else
        _pendingTiles.insert(cacheID,new QMap<quint32,QImage *>());


    //Заказывайте плитки у всех наших прекрасных детей
    for (int i = 0; i < _childSources.size(); i++)
    {
        QSharedPointer<MapTileSource> child = _childSources.at(i);
        child->requestTile(x,y,z);
    }
}

//private slot
void CompositeTileSource::handleTileRetrieved(quint32 x, quint32 y, quint8 z)
{
    QMutexLocker lock(_globalMutex);
    QObject * sender = QObject::sender();
    MapTileSource * tileSource = qobject_cast<MapTileSource *>(sender);

    //Убедитесь, что этот слот был вызван по сигналу с MapTileSource
    if (!tileSource)
    {
        qWarning() << this << "failed MapTileSource cast";
        return;
    }


    //Убедитесь, что это уведомление от источника MapTileSource, о котором мы заботимся
    int tileSourceIndex = -1;
    for (int i = 0; i < _childSources.size(); i++)
    {
        if (_childSources[i].data() != tileSource)
            continue;
        tileSourceIndex = i;
        break;
    }

    if (tileSourceIndex == -1)
    {
        qWarning() << this << "received tile from unknown source...";
        return;
    }

    //Убедитесь, что это именно та плитка, которая нас интересует
    const QString cacheID = MapTileSource::createCacheID(x,y,z);
    if (!_pendingTiles.contains(cacheID))
    {
        qWarning() << this << "received unknown tile" << x << y << z << "from" << tileSource;
        return;
    }

    //Убедитесь, что плитка ненулевая
    QImage * tile = tileSource->getFinishedTile(x,y,z);
    if (!tile)
    {
        qWarning() << this << "received null tile" << x << y << z << "from" << tileSource;
        return;
    }

    //qDebug() << this << "Retrieved tile" << x << y << z << "from" << tileSource;

    /*
      Поместите плитку в нашу структуру отложенных плиток. Если это была последняя плитка, которую мы хотели, постройте
      наш готовый продукт и уведомить нашего клиента. Если мы уже получили этот титул, потому что
      по какой-то причине это было запрошено дважды (например, сумасшедшее увеличение / уменьшение масштаба), тогда давайте просто продолжим
      и удаляем новую версию, и занимаемся своими делами.
    */
    QMap<quint32, QImage *> * tiles = _pendingTiles.value(cacheID);
    if (tiles->contains(tileSourceIndex))
    {
        delete tile;
        return;
    }
    tiles->insert(tileSourceIndex,tile);


    //Все еще ждете плитку или две?
    if (tiles->size() < _childSources.size())
        return;


    //Время для укладки готовой композитной плитки
    QImage * toRet = new QImage(this->tileSize(),
                                this->tileSize(),
                                QImage::Format_ARGB32_Premultiplied);
    QPainter painter;
    painter.begin(toRet);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setOpacity(1.0);
    for (int i = tiles->size()-1; i >= 0; i--)
    {
        QImage * childTile = tiles->value(i);
        qreal opacity = _childOpacities[i];


        //Если нет других слоев, нам нужно быть непрозрачными, несмотря ни на что
        if (this->numSources() == 1)
            opacity = 1.0;

        if (_childEnabledFlags[i] == false)
            opacity = 0.0;
        painter.setOpacity(opacity);
        painter.drawImage(0,0,*childTile);
        delete childTile;
    }
    delete tiles;
    _pendingTiles.remove(cacheID);
    painter.end();

    this->prepareNewlyReceivedTile(x,y,z,toRet);
}

//private slot
void CompositeTileSource::clearPendingTiles()
{
    QList<QMap<quint32, QImage *> * > pendingTiles = _pendingTiles.values();
    for (int i = 0; i < pendingTiles.size(); i++)
    {
        QMap<quint32, QImage *> * tiles = pendingTiles.at(i);
        Q_FOREACH(QImage * tile, tiles->values())
            delete tile;
        delete tiles;
    }
    _pendingTiles.clear();
}

//private
void CompositeTileSource::doChildThreading(QSharedPointer<MapTileSource> source)
{
    if (source.isNull())
        return;

    //We create a new thread for each child source in our care.
    //Мы создаем новый поток для каждого дочернего источника, находящегося в нашем ведении.
    QThread * sourceThread = new QThread();
    sourceThread->start();
    source->moveToThread(sourceThread);

    //Set the thread up so that it will shutdown and be destroyed when the tilesource is dead
    ////Настройте поток так, чтобы он завершал работу и был уничтожен, когда источник плитки будет отключен
    connect(source.data(),
            &QObject::destroyed,
            sourceThread,
            &QThread::quit);
    connect(sourceThread,
            &QThread::finished,
            sourceThread,
            &QObject::deleteLater);
}

const QList<QSharedPointer<MapTileSource> > &CompositeTileSource::childSources() const
{
    return _childSources;
}


void CompositeTileSource::setMapType(typeMaps newType)
{

    typeMap = newType;

    for(auto &it:_childSources)
    {
        it->setMapType(newType);
    }

}

