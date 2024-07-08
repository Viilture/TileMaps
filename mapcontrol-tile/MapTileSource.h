#ifndef MAPTILESOURCE_H
#define MAPTILESOURCE_H

#include <QObject>
#include <QPoint>
#include <QPointF>
#include <QImage>
#include <QCache>
#include <QMutex>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QHash>

#include "MapGraphics_global.h"

//Тип карты
enum typeMaps
{
    osmMaps = 0,
    googleMaps = 1,
    googleGEOMaps = 2,
    statmen,
    openToppoMap,

};

class  MapTileSource : public QObject
{
    Q_OBJECT

    const QString MAPGRAPHICS_CACHE_FOLDER_NAME = QStringLiteral("/MapCache/Barier2MapCache/");

public:
    /**
     * @brief Enum используется для описания того, как MapTileSource должен кэшировать тайлы, если вообще должен. NoCaching не
     * тайлы кеша вообще. DiskAndMemCaching кэширует тайлы на диске и в оперативной памяти
     *
     */
    enum CacheMode
    {
        NoCaching,
        DiskAndMemCaching
    };

public:
    explicit MapTileSource(QObject *parent = nullptr);
    virtual ~MapTileSource();

    /**
     * @brief Заставляет MapTileSource запрашивать плитку (x,y) с уровнем масштабирования z.
     * Сигнал tileRetrieved будет излучаться, когда плитка будет доступна, и в этот момент ее можно будет
     * получено с помощью getFinishedTile()
     *
     * @param x
     * @param y
     * @param z
     */
    void requestTile(quint32 x, quint32 y, quint8 z);

    /**
     * @brief Извлекает указатель на полученный фрагмент изображения. Вы должны вызвать requestTile и дождаться
     * Сигнал tileRetrieved перед вызовом этого метода. Возвращает указатель QImage в случае успеха, null в случае неудачи.
     * Вызывающий объект становится владельцем указателя QImage, т. е. вызывающий отвечает за его удаление.
     *
     * @param x
     * @param y
     * @param z
     * @return QImage
     */
    QImage * getFinishedTile(quint32 x, quint32 y, quint8 z);

    MapTileSource::CacheMode cacheMode() const;

    void setCacheMode(MapTileSource::CacheMode);

    /**
     * @brief Преобразование географических (широта, долгота) координат в координаты QGraphicsScene. MapTileSource
     * реализация должна реализовать этот метод.
     *
     * @param ll lat,lon для преобразования
     * @param zoomLevel уровень масштабирования, используемый для преобразования
     * @return QPointF точка в координатах QGraphicsScene
     */
    virtual QPointF ll2qgs(const QPointF& ll, quint8 zoomLevel) const=0;

    /**
     * @brief Преобразует координаты QGraphicsScene в географические (широта, долгота) координаты. MapTileSource
     * реализация должна реализовать этот метод.
     *
     * @param qgs точка в координатах пикселя (QGraphicsScene)
     * @param zoomLevel уровень масштабирования, используемый для преобразования
     * @return QPointF точка в географических (широта, долгота) координатах
     */
    virtual QPointF qgs2ll(const QPointF& qgs, quint8 zoomLevel) const=0;

    /**
     * @brief Чисто виртуальный метод, который возвращает количество плиток на заданном уровне масштабирования.
     *
     * @параметр масштабирования
     * @вернуть quint64
     */
    virtual quint64 tilesOnZoomLevel(quint8 zoomLevel) const=0;

    /**
     * @brief Возвращает размер фрагментов карты в пикселях. Плитки считаются квадратами длиной
     * и ширина, возвращаемая этим методом. Реализация MapTileSource должна реализовать этот метод.
     *
     * @return quint16 длина/ширина фрагментов карты
     */
    virtual quint16 tileSize() const=0;

    /**
     * @brief Возвращает минимальный уровень масштабирования (наиболее уменьшенный), доступный для данной широты/долготы. А
     * Реализация MapTileSource должна реализовать этот метод.
     *
     * @параметр ll
     * @вернуть quint8
     */
    virtual quint8 minZoomLevel(QPointF ll = QPointF())=0;

    /**
     * @brief Возвращает максимальный уровень масштабирования (наиболее приближенный), доступный для данной широты/долготы. А
     * Реализация MapTileSource должна реализовать этот метод.
     *
     * @параметр ll
     * @вернуть quint8
     */
    virtual quint8 maxZoomLevel(QPointF ll = QPointF())=0;

    /**
     * @brief Возвращает имя этого MapTileSource. Реализация MapTileSource должна реализовать это
     * метод.
     *
     * @возврат QString
     */
    virtual QString name() const=0;

    /**
     * @brief Возвращает расширение файла (jpg, png и т. д.) фрагментов изображения, возвращенных источником. Не
     * включить . в расширении. например, вернуть jpg вместо .jpg. Реализация MapTileSource должна
     * реализовать этот метод.
     */
    virtual QString tileFileExtension() const=0;
    
    void setMAPGRAPHICS_CACHE_FOLDER_NAME(const QString &newMAPGRAPHICS_CACHE_FOLDER_NAME);

    const QString &getMAPGRAPHICS_CACHE_FOLDER_NAME() const;

Q_SIGNALS:
    /**
     * @brief Сигнал выдается, когда тайл, запрошенный с помощью requestTile(), был получен и может
     * можно получить с помощью getFinishedTile()
     *
     * @param x
     * @param y
     * @param z
     */
    void tileRetrieved(quint32 x, quint32 y, quint8 z);

    /**
     * @brief Сигнал выдается при запросе плитки с помощью requestTile().
     *
     * @param x
     * @param y
     * @param z
     */
    void tileRequested(quint32 x, quint32 y, quint8 z);

    /*!
       \brief Генерируется, когда жизненно важные параметры источника тайлов изменились, и любой, кто отображает тайлы, должен
       обновить.
    */
    void allTilesInvalidated();
    
public Q_SLOTS:
    /*! \brief Устанавлиаем тип карты */
    virtual void setMapType(typeMaps newType = typeMaps::googleMaps) = 0;



private Q_SLOTS:
    void startTileRequest(quint32 x, quint32 y, quint8 z);
    void clearTempCache();

protected:

    /*!  \brief Путь до файлов карты */
    //QString MAPGRAPHICS_CACHE_FOLDER_NAME = "MapCache/Barier2MapCache";

    /*!  \brief Тип карты*/
    typeMaps typeMap = typeMaps::googleGEOMaps;

    /**
     * @brief Этот статический метод берет x,y,z плитки и создает уникальную строку, которая используется
     * в качестве ключа в тайниках для отслеживания плитки.
     *
     * @param x x-координата тайла
     * @param y y-координата тайла
     * @param z уровень масштабирования тайла
     * @return QString уникальный идентификатор кеша
     */
    static QString createCacheID(quint32 x, quint32 y, quint8 z);

    /**
     * @brief Этот статический удобный метод принимает cacheID и помещает числовые значения x,y,z, которые
     * сгенерировал cacheID в переменные, на которые указывают x, y, z. Возвращает true в случае успеха, false в случае неудачи.
     *
     * @param string идентификатор кеша, который вы конвертируете
     * @param x, где вы хотите значение x
     * @param y, где вы хотите значение y
     * @param z, где вы хотите значение z
     * @вернуть логическое значение
     */
    static bool cacheID2xyz(const QString & string, quint32 * x, quint32 * y, quint32 * z);

    /**
     * @brief Учитывая идентификатор кеша, извлеките тайл с этим идентификатором кеша из memcache. Возвращает указатель
     * в QImage в случае успеха, null в случае неудачи. Вызывающий берет на себя ответственность за удаление возвращенного
     * QИзображение
     *
     * @param cacheID cacheID тайла, который вы хотите получить из кеша
     * @возврат QImage
     */
    QImage * fromMemCache(const QString& cacheID);

    /**
     * @brief Учитывая cacheID и указатель на QImage, вставляет QImage, на который указывает указатель, в
     * кэш памяти с использованием cacheID в качестве ключа
     *
     * @param идентификатор кеша
     * @параметр ккеша
     */
    void toMemCache(const QString& cacheID, QImage * toCache, const QDateTime &expireTime = QDateTime());

    /**
     * @brief Получив идентификатор кеша, извлеките тайл с этим ИД кеша из дискового кеша. Возвращает
     * указатель на QImage в случае успеха, null в случае неудачи. Вызывающий берет на себя ответственность за удаление
     * возвращен QImage
     *
     * @param cacheID cacheID тайла, который вы хотите получить из кеша
     * @возврат QImage
     */
    QImage * fromDiskCache(const QString& cacheID);

    /**
     * @brief Учитывая cacheID и указатель на QImage, вставляет QImage, на который указывает указатель, в
     * кеш диска с использованием cacheID в качестве ключа.
     * Опционально принимает объект QDateTime, указывающий время, в течение которого QImage следует хранить в кэше.
     * до. По умолчанию 7 дней.
     *
     * @param идентификатор кеша
     * @параметр ккеша
     * @param cacheUntil
     */
    void toDiskCache(const QString& cacheID, QImage * toCache, const QDateTime &expireTime = QDateTime());

    /**
     * @brief Извлекает (из MapQuest или OSM или откуда-то еще) или создает плитку, если она не кэшируется.
     * Вот где резина отправляется в путь, так сказать, для MapTileSource. В случае успеха этот метод
     * следует просто вызвать prepareRetrievedTile. В случае неудачи ничего не делать.
     *
     * @param x x-координата тайла
     * @param y y-координата тайла
     * @param z уровень масштабирования тайла
     * @вернуть логическое значение
     */
    virtual void fetchTile(quint32 x,
                           quint32 y,
                           quint8 z)=0;

    //Call only for tiles which were newly-generated or newly-acquired from the network (i.e., not cached)
    void prepareNewlyReceivedTile(quint32 x, quint32 y, quint8 z, QImage * image, QDateTime expireTime = QDateTime());

    /**
     * @brief Возвращает время, когда тайл должен истечь из любого кэша.
     * Это следует вызывать только для плиток, которые фактически кэшированы!
     * @param cacheID — cacheID тайла.
     * @return QDateTime истечения срока действия тайла (время, после которого он должен быть повторно запрошен или перегенерирован)
     */
    QDateTime getTileExpirationTime(const QString& cacheID);

    /**
      * @brief Устанавливает время, когда тайл должен истечь из любых кешей.
      * @param cacheID тайла
      * @param QDateTime истечения срока действия плитки (время, после которого она должна быть повторно запрошена или перегенерирована)
     */
    void setTileExpirationTime(const QString& cacheID, QDateTime expireTime);

private:
    /**
     * @brief prepareRetrievedTile prepares a generated/retrieve tile for retrieval by the client
     * and notifies the client that the tile is ready.
     */
    void prepareRetrievedTile(quint32 x, quint32 y, quint8 z, QImage * image);

    /**
     * @brief Given the x,y, and z of a tile, returns the directory where it should be cached on disk
     *
     * @param x
     * @param y
     * @param z
     * @return QDir
     */
    QDir getDiskCacheDirectory(quint32 x, quint32 y, quint8 z) const;

    /**
     * @brief Given the x,y, and z of a tile, returns the full path to the file where it should be
     * cached on disk
     *
     * @param x
     * @param y
     * @param z
     * @return QString
     */
    QString getDiskCacheFile(quint32 x, quint32 y, quint8 z) const;

    /*!
     \brief Loads cache expiration times from disk if necessary
    */
    void loadCacheExpirationsFromDisk();

    void saveCacheExpirationsToDisk();

    bool _cacheExpirationsLoaded;

    /*
      loadCacheExpirationsFromDisk() stores the place where it loaded from here.
      When we're destructing and need to serialize out we can use this instead of
      foolishly trying to call a pure-virtual method (name()) from the destructor
    */
    QString _cacheExpirationsFile;

    MapTileSource::CacheMode _cacheMode;

    //Temporary cache for QImage tiles waiting for the client to take them
    QCache<QString, QImage> _tempCache;
    QMutex _tempCacheLock;

    //The "real" cache, where tiles are saved in memory so we don't download them again
    QCache<QString, QImage> _memoryCache;

    QHash<QString, QDateTime> _cacheExpirations;
    
};

#endif // MAPTILESOURCE_H
