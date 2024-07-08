#include "MapTileSource.h"

#include <QStringBuilder>
#include <QMutexLocker>
#include <QtDebug>
#include <QStringList>
#include <QDataStream>


const quint32 DEFAULT_CACHE_DAYS = 10000;
const quint64 MAX_DISK_CACHE_READ_ATTEMPTS = 100000;


/*
 * Этот код определяет базовый класс MapTileSource, который предоставляет интерфейс для загрузки тайлов карты из различных источников.
   Класс использует кэширование тайлов в оперативной памяти и на диске для повышения производительности. При запросе тайла метод requestTile() генерирует сигнал tileRequested(),
   который запускает загрузку тайла в отдельном потоке. Загруженный тайл хранится во временном кэше _tempCache, который очищается при сигнале allTilesInvalidated().
   Метод getFinishedTile() возвращает загруженный тайл из временного кэша. Если запрашиваемый тайл не был загружен, метод выдаст предупреждение и вернет 0.
   Класс также имеет методы для сохранения/загрузки информации об истечении времени кэша на диск.
*/

MapTileSource::MapTileSource(QObject *parent) :
    QObject(parent), _cacheExpirationsLoaded(false)
{
    this->setCacheMode(DiskAndMemCaching);

    // Мы подключаем эту пару сигнал/слот для связи между потоками.
    connect(this,
            &MapTileSource::tileRequested,
            this,
            &MapTileSource::startTileRequest,
            Qt::QueuedConnection);

    /*
       Когда все наши плитки будут признаны недействительными, мы очищаем наш временный кеш, чтобы любые дезинформированные клиенты
       тот, кто не заметит, получит нулевую плитку вместо старой плитки.
    */
    connect(this,
            &MapTileSource::allTilesInvalidated,
            this,
            &MapTileSource::clearTempCache);

    //MAPGRAPHICS_CACHE_FOLDER_NAME = QDir::homePath() % "/" % "MapCache/Barier2MapCache" % "/";
}

MapTileSource::~MapTileSource()
{
    this->saveCacheExpirationsToDisk();
}

void MapTileSource::requestTile(quint32 x, quint32 y, quint8 z)
{
    /* Мы посылаем сигнал для связи между потоками. MapTileSource (обычно) работает в собственном
       поток, но этот метод будет вызываться из другого потока (вероятно, из потока GUI).
       Легко общаться между потоками с поставленными в очередь сигналами/слотами.
    */
    Q_EMIT this->tileRequested(x,y,z);
}

QImage *MapTileSource::getFinishedTile(quint32 x, quint32 y, quint8 z)
{
    const QString cacheID = MapTileSource::createCacheID(x,y,z);
    QMutexLocker lock(&_tempCacheLock);
    if (!_tempCache.contains(cacheID))
    {
        qWarning() << "getFinishedTile() is called but the tile is missing";
        return 0;
    }
    return _tempCache.take(cacheID);
}

/*
 * Этот код содержит реализацию нескольких методов класса MapTileSource.
   Метод cacheMode() возвращает текущий режим кэширования тайлов.
   Метод setCacheMode() устанавливает режим кэширования тайлов.
   Приватный слот startTileRequest() вызывается при запросе тайла методом requestTile(). Сначала метод проверяет наличие запрошенного тайла в кэшах.
   Если тайл был найден, он готовится для возврата клиенту и метод завершается. Иначе метод вызывает защищенный метод fetchTile() для загрузки тайла.
   Приватный слот clearTempCache() очищает временный кэш _tempCache.
   Защищенный статический метод createCacheID() создает уникальный идентификатор кэша для тайла на основе его координат.
   Защищенный статический метод cacheID2xyz() преобразует строку идентификатора кэша в координаты тайла x, y и z. Если преобразование не удалось, метод возвращает false.
 * */

MapTileSource::CacheMode MapTileSource::cacheMode() const
{
    return _cacheMode;
}

void MapTileSource::setCacheMode(MapTileSource::CacheMode nMode)
{
    _cacheMode = nMode;
}



//private slot
void MapTileSource::startTileRequest(quint32 x, quint32 y, quint8 z)
{
    //Сначала проверяем кеши для плитки
    if (this->cacheMode() == DiskAndMemCaching)
    {
        const QString cacheID = MapTileSource::createCacheID(x,y,z);
        QImage * cached = this->fromMemCache(cacheID);
        if (!cached)
            cached = this->fromDiskCache(cacheID);

        //Если мы получили изображение из одного из кешей, подготавливаем его для клиента и возвращаем
        if (cached)
        {
            this->prepareRetrievedTile(x,y,z,cached);
            return;
        }
    }

    //Если мы сюда попали, тайл не был закеширован и мы должны попытаться его получить
    this->fetchTile(x,y,z);
}

//private slot
void MapTileSource::clearTempCache()
{
    _tempCache.clear();
}

const QString &MapTileSource::getMAPGRAPHICS_CACHE_FOLDER_NAME() const
{
    return MAPGRAPHICS_CACHE_FOLDER_NAME;
}

void MapTileSource::setMAPGRAPHICS_CACHE_FOLDER_NAME(const QString &newMAPGRAPHICS_CACHE_FOLDER_NAME)
{
    //MAPGRAPHICS_CACHE_FOLDER_NAME = newMAPGRAPHICS_CACHE_FOLDER_NAME;
    Q_UNUSED(newMAPGRAPHICS_CACHE_FOLDER_NAME)
}

//protected static
QString MapTileSource::createCacheID(quint32 x, quint32 y, quint8 z)
{
    //Мы используем %, потому что конкатенация с помощью QStringBuilder более эффективна.
    QString toRet = QString::number(x) % "," % QString::number(y) % "," % QString::number(z);
    return toRet;
}

//protected static
bool MapTileSource::cacheID2xyz(const QString & string, quint32 *x, quint32 *y, quint32 *z)
{
    QStringList list = string.split(',');
    if (list.size() != 3)
    {
        qWarning() << "Invalid cache ID" << string << "can't convert";
        return false;
    }

    bool ok = true;
    *x = list.at(0).toUInt(&ok);
    if (!ok)
        return false;
    *y = list.at(1).toUInt(&ok);
    if (!ok)
        return false;
    *z = list.at(2).toUInt(&ok);
    return ok;
}


/*
 * Этот код содержит реализацию нескольких методов класса MapTileSource.
   Метод cacheMode() возвращает текущий режим кэширования тайлов.
   Метод setCacheMode() устанавливает режим кэширования тайлов.
   Приватный слот startTileRequest() вызывается при запросе тайла методом requestTile(). Сначала метод проверяет наличие запрошенного тайла в кэшах.
   Если тайл был найден, он готовится для возврата клиенту и метод завершается. Иначе метод вызывает защищенный метод fetchTile() для загрузки тайла.
   Приватный слот clearTempCache() очищает временный кэш _tempCache.
   Защищенный статический метод createCacheID() создает уникальный идентификатор кэша для тайла на основе его координат.
   Защищенный статический метод cacheID2xyz() преобразует строку идентификатора кэша в координаты тайла x, y и z. Если преобразование не удалось, метод возвращает false.
 * */

QImage *MapTileSource::fromMemCache(const QString &cacheID)
{
    QImage * toRet = 0;

    if (_memoryCache.contains(cacheID))
    {
        //Определяем, когда тайл, который мы загружаем из кеша, должен был истечь
        QDateTime expireTime = this->getTileExpirationTime(cacheID);

        //Если кешированный тайл старше, чем хотелось бы, выбрасываем его
        if (QDateTime::currentDateTimeUtc().secsTo(expireTime) <= 0)
        {
            _memoryCache.remove(cacheID);
        }
        //Иначе делаем копию тайла из кеша и возвращаем вызывающему
        else
        {
            toRet = new QImage(*_memoryCache.object(cacheID));
        }
    }

    return toRet;
}

void MapTileSource::toMemCache(const QString &cacheID, QImage *toCache, const QDateTime &expireTime)
{
    if (toCache == 0)
        return;

    if (_memoryCache.contains(cacheID))
        return;

    //Обратите внимание, когда тайл истечет
    this->setTileExpirationTime(cacheID, expireTime);

    // Делаем копию QImage
    QImage * copy = new QImage(*toCache);
    _memoryCache.insert(cacheID,copy);
}

QImage *MapTileSource::fromDiskCache(const QString &cacheID)
{
    //Определяем x,y,z на основе cacheID
    quint32 x,y,z;
    if (!MapTileSource::cacheID2xyz(cacheID,&x,&y,&z))
        return 0;

    //Проверяем, есть ли он у нас в кеше
    const QString path = this->getDiskCacheFile(x,y,z);
    QFile fp(path);
    if (!fp.exists())
        return 0;

    //Определяем, когда тайл, который мы загружаем из кеша, должен был истечь
    QDateTime expireTime = this->getTileExpirationTime(cacheID);

    //Если кешированный тайл старше, чем хотелось бы, выбрасываем его
    if (QDateTime::currentDateTimeUtc().secsTo(expireTime) <= 0)
    {
        if (!QFile::remove(path))
            qWarning() << "Failed to delete old cache file" << path;
        return 0;
    }

    if (!fp.open(QFile::ReadOnly))
    {
        qWarning() << "Failed to open" << QFileInfo(fp.fileName()).baseName() << "from cache";
        return 0;
    }

    QByteArray data;
    quint64 counter = 0;
    while (data.length() < fp.size())
    {
        data += fp.read(20480);
        if (++counter >= MAX_DISK_CACHE_READ_ATTEMPTS)
        {
            qWarning() << "Reading the cache file" << fp.fileName() << "took too long. Interrupt.";
            return 0;
        }
    }

    QImage * image = new QImage();
    if (!image->loadFromData(data))
    {
        delete image;
        return 0;
    }

    return image;
}

/*
 *
 * Этот код содержит реализацию нескольких методов класса MapTileSource.
   Метод toDiskCache() сохраняет тайл в дисковый кэш. Метод получает уникальный идентификатор кэша, извлекает координаты тайла из этого идентификатора и определяет путь к файлу,
   в котором будет сохранен кэш. Если кэш уже существует, метод завершается без сохранения. Иначе метод записывает изображение в файл.
   Метод prepareRetrievedTile() готовит изображение тайла, полученное из сети или из кэша, для возврата клиенту. Метод помещает изображение во временный кэш _tempCache,
   который клиент может получить с помощью метода getFinishedTile(). Метод также отправляет сигнал tileRetrieved(), чтобы клиент знал, что изображение готово для получения.
   Метод prepareNewlyReceivedTile() готовит вновь полученное изображение тайла для сохранения в кэше и для возврата клиенту. Метод вызывает метод prepareRetrievedTile()
   для добавления тайла во временный кэш. Если режим кэширования задан как DiskAndMemCaching, метод также сохраняет тайл в памятном и дисковом кэше.
   Метод getTileExpirationTime() возвращает время истечения срока действия кэша тайла по его идентификатору. Метод загружает информацию об истечении сроков кэша из файла,
   если это еще не было сделано. Если информация об истечении сроков для данного идентификатора не найдена, метод возвращает время истечения срока действия по умолчанию.
   Метод setTileExpirationTime() устанавливает время истечения срока действия кэша тайла по его идентификатору. Метод загружает информацию об истечении сроков кэша из файла,
   если это еще не было сделано. Если время истечения срока действия не задано, метод устанавливает время истечения по умолчанию. Затем метод сохраняет время истечения срока действия в файл.
 * */

void MapTileSource::toDiskCache(const QString &cacheID, QImage *toCache, const QDateTime &expireTime)
{
    //Определяем x,y,z на основе cacheID
    quint32 x,y,z;
    if (!MapTileSource::cacheID2xyz(cacheID,&x,&y,&z))
        return;

    //Узнаем, где мы будем кэшировать
    const QString filePath = this->getDiskCacheFile(x,y,z);

    //Если мы уже что-то закэшировали, больше не кешируем
    QFile fp(filePath);
    if (fp.exists())
        return;

    //Обратите внимание, когда тайл истечет
    this->setTileExpirationTime(cacheID, expireTime);

    //Автоматическое определение формата файла
    const char * format = 0;

    // Нет сжатия для типов файлов с потерями!
    const int quality = 100;

    //Попытаемся записать данные
    if (!toCache->save(filePath,format,quality))
        qWarning() << "Failed to put" << this->name() << x << y << z << "to disk cache";
}

void MapTileSource::prepareRetrievedTile(quint32 x, quint32 y, quint8 z, QImage *image)
{
    //Проверить работоспособность плитки здесь необязательно
    if (image == 0)
        return;

    // Поместите его во "временный поисковый кеш", чтобы пользователь мог его получить
    QMutexLocker lock(&_tempCacheLock);
    _tempCache.insert(MapTileSource::createCacheID(x,y,z),
                      image);
    /*
      Мы должны явно разблокировать мьютекс перед отправкой tileRetrieved в случае, если
      мы работаем в потоке графического интерфейса (поскольку сигнал может вызвать
      слоты сразу в этом случае).
     */
    lock.unlock();

    //Отправляем сигнал, чтобы пользователь знал, что надо вызывать getFinishedTile()
    Q_EMIT this->tileRetrieved(x,y,z);
}

void MapTileSource::prepareNewlyReceivedTile(quint32 x, quint32 y, quint8 z, QImage *image, QDateTime expireTime)
{
    // Вставить в кеши, если применимо
    const QString cacheID = MapTileSource::createCacheID(x,y,z);
    if (this->cacheMode() == DiskAndMemCaching)
    {
        this->toMemCache(cacheID, image, expireTime);
        this->toDiskCache(cacheID, image, expireTime);
    }

    //Помещаем плитку в доступное для клиентов место и уведомляем их
    this->prepareRetrievedTile(x, y, z, image);
}

//protected
QDateTime MapTileSource::getTileExpirationTime(const QString &cacheID)
{
    //Убедимся, что у нас загружена база данных истечения срока действия
    this->loadCacheExpirationsFromDisk();

    QDateTime expireTime;
    if (_cacheExpirations.contains(cacheID))
        expireTime = _cacheExpirations.value(cacheID);
    else
    {
       // qWarning() << "Tile" << cacheID << "has an unknown expiration time. Reset to default" << DEFAULT_CACHE_DAYS << "days.";
        expireTime = QDateTime::currentDateTimeUtc().addDays(DEFAULT_CACHE_DAYS);
        _cacheExpirations.insert(cacheID,expireTime);
    }

    return expireTime;
}

//protected
void MapTileSource::setTileExpirationTime(const QString &cacheID, QDateTime expireTime)
{
    //Убедимся, что у нас загружена база данных истечения срока действия
    this->loadCacheExpirationsFromDisk();

    //Если нам сказали, когда истечет срок действия плитки, сохраните этот срок. В противном случае используйте значение по умолчанию.
    if (expireTime.isNull())
    {
        expireTime = QDateTime::currentDateTimeUtc().addDays(DEFAULT_CACHE_DAYS);
    }

    _cacheExpirations.insert(cacheID, expireTime);
}


/*
 * Этот код содержит дополнительную реализацию нескольких методов класса MapTileSource.
   Метод getDiskCacheDirectory() возвращает объект QDir для директории дискового кэша тайла с заданными координатами. Метод создает директорию, если она не существует.
   Корневая директория для всех директорий кэша находится в домашней директории пользователя.
   Метод getDiskCacheFile() возвращает полный путь к файлу дискового кэша для тайла с заданными координатами. Метод вызывает getDiskCacheDirectory() для получения родительской
   директории и добавляет имя файла, которое состоит из координат тайла и расширения файла.
   Методы loadCacheExpirationsFromDisk() и saveCacheExpirationsToDisk() загружают и сохраняют информацию об истечении сроков кэша в файле cacheExpirations.db. Метод loadCacheExpirationsFromDisk()
   загружает информацию из файла, если он существует, и сохраняет ее в члене класса _cacheExpirations. Метод saveCacheExpirationsToDisk() сохраняет информацию из _cacheExpirations в файл cacheExpirations.db.
   Если файл не существует, он будет создан. Если информация не была загружена из файла, метод saveCacheExpirationsToDisk() ничего не делает.
   Члены класса _cacheExpirationsLoaded и _cacheExpirationsFile являются вспомогательными переменными для отслеживания загрузки информации об истечении сроков кэша из файла и хранения пути к
   файлу cacheExpirations.db, соответственно.
 * */

//private
QDir MapTileSource::getDiskCacheDirectory(quint32 x, quint32 y, quint8 z) const
{
    Q_UNUSED(y)

    QString pathString = QDir::homePath() % "/" % MAPGRAPHICS_CACHE_FOLDER_NAME % "/" % this->name() % "/" % QString::number(z) % "/" % QString::number(x);

    QDir toRet = QDir(pathString);
    if (!toRet.exists())
    {
        if (!toRet.mkpath(toRet.absolutePath()))
            qWarning() << "Failed to create cache directory" << toRet.absolutePath();
    }
    return toRet;
}

//private
QString MapTileSource::getDiskCacheFile(quint32 x, quint32 y, quint8 z) const
{
    QString toRet = this->getDiskCacheDirectory(x,y,z).absolutePath() % "/" % QString::number(y) % "." % this->tileFileExtension();

    return toRet;
}

//private
void MapTileSource::loadCacheExpirationsFromDisk()
{
    if (_cacheExpirationsLoaded)
        return;

    //Если мы попытаемся сделать это и преуспеем или даже потерпим неудачу, больше не пробуйте
    _cacheExpirationsLoaded = true;

    QDir dir = this->getDiskCacheDirectory(0,0,0);
    QString path = dir.absolutePath() % "/" % "cacheExpirations.db";
    _cacheExpirationsFile = path;

    QFile fp(path);
    if (!fp.exists())
        return;

    if (!fp.open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open cache expiration file for reading:" << fp.errorString();
        return;
    }

    QDataStream stream(&fp);
    stream >> _cacheExpirations;
}

void MapTileSource::saveCacheExpirationsToDisk()
{
    if (!_cacheExpirationsLoaded || _cacheExpirationsFile.isEmpty())
        return;

    QFile fp(_cacheExpirationsFile);

    if (!fp.open(QIODevice::Truncate | QIODevice::WriteOnly))
    {
        qWarning() << "Failed to open cache expiration file for writing:" << fp.errorString();
        return;
    }

    QDataStream stream(&fp);
    stream << _cacheExpirations;

}
