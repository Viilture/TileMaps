#include "MapTileGraphicsObject.h"

#include <QPainter>
#include <QtDebug>

MapTileGraphicsObject::MapTileGraphicsObject(quint16 tileSize, QObject *parent)
{
    Q_UNUSED(parent);

    this->setTileSize(tileSize);
    _tile = 0;
    _tileX = 0;
    _tileY = 0;
    _tileZoom = 0;
    _initialized = false;
    _havePendingRequest = false;


    //Значение z по умолчанию важно --- используется в MapGraphicsView
    this->setZValue(-1.0);
}

MapTileGraphicsObject::~MapTileGraphicsObject()
{
    if (_tile != 0)
    {
        delete _tile;
        _tile = 0;
    }
}

QRectF MapTileGraphicsObject::boundingRect() const
{
    const quint16 size = this->tileSize();
    return QRectF(-1 * size / 2.0,
                  -1 * size / 2.0,
                  size,
                  size);
}

void MapTileGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)


    //Если у нас есть плитка, нарисуйте ее. В противном случае отобразится сообщение о загрузке или "Нет источника плитки"
    if (_tile != 0)
        painter->drawPixmap(this->boundingRect().toRect(),
                            *_tile);
    else
    {
        QString string;
        if (_tileSource.isNull())
            string = QLatin1String(" No tile source defined");
        else
            string = QStringLiteral(" Загрузка...");
        painter->drawText(this->boundingRect(),
                          string,
                          QTextOption(Qt::AlignCenter));
    }
}

quint16 MapTileGraphicsObject::tileSize() const
{
    return _tileSize;
}

void MapTileGraphicsObject::setTileSize(quint16 tileSize)
{
    if (tileSize == _tileSize)
        return;
    _tileSize = tileSize;
}

void MapTileGraphicsObject::setTile(quint32 x, quint32 y, quint8 z, bool force)
{

    //Не запрашивайте повторно ту же плитку, которую мы уже отображаем, если только force=true или _initialized=false
    if (_tileX == x && _tileY == y && _tileZoom == z && !force && _initialized)
        return;

    //Избавьтесь от старой плитки
    if (_tile != 0)
    {
        delete _tile;
        _tile = 0;
    }


    //Сохраните информацию для запрашиваемой нами плитки
    _tileX = x;
    _tileY = y;
    _tileZoom = z;
    _initialized = true;


    //Если у нас есть нулевой источник плитки, то делать больше нечего!
    if (_tileSource.isNull())
        return;


    //Если наш тайловый источник исправен, подключитесь к сигналу, который нам понадобится, чтобы получить результат после запроса
    connect(_tileSource.data(),
            &MapTileSource::tileRetrieved,
            this,
            &MapTileGraphicsObject::handleTileRetrieved);

    //Убедитесь, что мы знаем, что запрашиваем плитку
    _havePendingRequest = true;


    ////Запросить плитку из источника плитки, который выдаст полученную плитку по завершении
    //qDebug() << this << "requests" << x << y << z;
    _tileSource->requestTile(x,y,z);
}

QSharedPointer<MapTileSource> MapTileGraphicsObject::tileSource() const
{
    return _tileSource;
}

void MapTileGraphicsObject::setTileSource(QSharedPointer<MapTileSource> nSource)
{

    //Отключитесь от старого источника, если применимо
    if (!_tileSource.isNull())
    {
        QObject::disconnect(_tileSource.data(),
                            &MapTileSource::tileRetrieved,
                            this,
                            &MapTileGraphicsObject::handleTileRetrieved);
        QObject::disconnect(_tileSource.data(),
                            &MapTileSource::allTilesInvalidated,
                            this,
                            &MapTileGraphicsObject::handleTileInvalidation);
    }


    //Установите новый источник
    QSharedPointer<MapTileSource> oldSource = _tileSource;
    _tileSource = nSource;


    //Подключайте сигналы, если это необходимо
    if (!_tileSource.isNull())
    {
        connect(_tileSource.data(),
                &MapTileSource::allTilesInvalidated,
                this,
                &MapTileGraphicsObject::handleTileInvalidation);

        //Мы подключаем/отключаем сигнал "Извлечена плитка" по мере необходимости и не делаем этого здесь!
    }


    //Принудительное обновление из нового источника
    this->handleTileInvalidation();
}

//private slot
void MapTileGraphicsObject::handleTileRetrieved(quint32 x, quint32 y, quint8 z)
{

    //Если нас не интересуют извлеченные плитки (т.е. мы не запрашивали плитку), верните
    //На самом деле этого не должно произойти, так как сигнал /слот должен быть отключен
    if (!_havePendingRequest)
        return;

    //Если это не та плитка, которую мы ищем, верните
    else if (_tileX != x || _tileY != y || _tileZoom != z)
        return;


    //Теперь мы знаем, что наша плитка была извлечена MapTileSource. Нам просто нужно это получить.
    _havePendingRequest = false;


    //Убедитесь, что какой-нибудь злоумышленник не установил для нашего MapTileSource значение null, пока мы не смотрели...
    if (_tileSource.isNull())
        return;

    QImage * image = _tileSource->getFinishedTile(x,y,z);

    //Убедитесь, что кто-нибудь не подтолкнул нас к захвату нашей плитки
    if (image == 0)
    {
        qWarning() << "Failed to get tile" << x << y << z << "from MapTileSource";
        return;
    }


    //Преобразуйте изображение в QPixmap
    //Мы должны сделать это здесь, так как мы не можем использовать Qpixmap в потоках, отличных от GUI (т.е. MapTileSource)
    QPixmap * tile = new QPixmap();
    *tile = QPixmap::fromImage(*image);


    //Удалить изображение
    delete image;
    image = 0;


    //Убедитесь, что старая плитка была утилизирована. Если этого не произошло, сделайте это
    //На самом деле так и должно было быть, поэтому выведите предупреждение
    if (_tile != 0)
    {
        qWarning() << "Tile should be null, but isn't";
        delete _tile;
        _tile = 0;
    }


    //Установите новую плитку и принудительно перерисуйте
    _tile = tile;
    this->update();


    //Отключите наше соединение сигнала/слота с MapTileSource до тех пор, пока нам не понадобится выполнить другой запрос
    //Еще неизвестно, что лучше - постоянно подключаться / переподключаться повторно или просто фильтровать события
    QObject::disconnect(_tileSource.data(),
                        &MapTileSource::tileRetrieved,
                        this,
                        &MapTileGraphicsObject::handleTileRetrieved);
}

//private slot
void MapTileGraphicsObject::handleTileInvalidation()
{

    //Если мы еще не были инициализированы с соответствующей координатой плитки для выборки, не выполняйте принудительное обновление
    if (!_initialized)
        return;


    //Вызовите settle с force=true, чтобы принудительно обновить
    this->setTile(_tileX,_tileY,_tileZoom,true);
}
