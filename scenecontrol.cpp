#include "scenecontrol.h"

#include <QGLWidget>

#define Barier2INI "/Barier2INI"

sceneControl::sceneControl(QWidget *parent, QGraphicsScene *scene) : QGraphicsView(scene, parent)
{
    init();

    static QTimer timer;

    connect(&timer, &QTimer::timeout, this, [=](){

        static int w = 1;
        static int g = 1;

        switch (w) {

        case 1:
        {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 10);
            ++g;
            if(g > 100)
            {
                w = 2;
                g = 0;
                mapContr->setMapType(typeMaps::osmMaps);
            }
            break;
        }
        case 2:
        {
            verticalScrollBar()->setValue(verticalScrollBar()->value() - 10);
            ++g;
            if(g > 100)
            {
                w = 3;
                g = 0;
                mapContr->setMapType(typeMaps::googleGEOMaps);
            }
            break;
        }
        case 3:
        {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 10);
            ++g;
            if(g > 100) {
                w=4;
                g=0;
                mapContr->setMapType(typeMaps::googleMaps);
            }
            break;
        }
        case 4:
        {
            verticalScrollBar()->setValue(verticalScrollBar()->value() + 10);
            ++g;
            if(g > 100) {
                w=1;
                g=0;mapContr->setMapType(typeMaps::statmen);
            }
            break;
        }
        default: break;

        }

    });

    //    static double lat = 24, lon = 56, h = 0;
    //    static double x,y,z;
    //    calcul.WGS84PolartoRectangle(lon, lat, h, x, y, z);
    //    calcul.WGS84RectangletoPolar(x, y, z, lon, lat, h);
    //    qDebug() << "x - " << x << " y - " << y << " z - " << z;

    //    int n = 100000;
    //    while(n--)
    //    {
    //        calcul.WGS84PolartoRectangle(lon, lat, h, x, y, z);
    //        calcul.WGS84RectangletoPolar(x, y, z, lon, lat, h);
    //    }

    //    qDebug() << "x - " << x << " y - " << y << " z - " << z;

}

void sceneControl::setCordSystem(CoordinatSystem newCordSystem)
{
    cordSystem = newCordSystem;
}

CoordinatSystem sceneControl::getCordSystem() const
{
    return cordSystem;
}


sceneControl::~sceneControl()
{

}

QGraphicsScene *sceneControl::getScene() const
{
    return this->scene();
}

void sceneControl::addMapControl(QSharedPointer<MapControl> &newMapControl)
{
    mapContr = newMapControl;
}

void sceneControl::init()
{
    setLineWidth(0);
    setMidLineWidth(0);
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::StrongFocus);
    setCacheMode(QGraphicsView::CacheBackground);
    setAttribute(Qt::WA_AcceptTouchEvents);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Установка палетты
    globalPalette   gPalette;
    dark_Palette  = gPalette.getDarkPalette();
    green_Palette = gPalette.getGreenPalette();

    QGraphicsScene *scene = new QGraphicsScene();
    if(scene != nullptr)
        setScene(scene);

    // класс управления тайловой картой
    mapContr = QSharedPointer<MapControl>::create(this);

    //Восстанавливаем тип карты и координаты из ini файла
    set = QSharedPointer<SettingsBR>::create();
    //  set->setValue(QStringLiteral("Map/useOpenGL"), false);

    useOpenGL = set->value(QStringLiteral("Map/useOpenGL"), false).toBool();
    if(useOpenGL)
        setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    else
        setViewportUpdateMode(FullViewportUpdate);

    setMouseTracking(true);
    viewport()->setMouseTracking(true);

    //Создаем элементы отображения информации на карте
    //Закругляем углы
    QPainterPath paintPath150;
    paintPath150.addRoundedRect(QRect(0,0,170,25), 10, 10, Qt::AbsoluteSize);
    QPainterPath paintPath100;
    paintPath100.addRoundedRect(QRect(0,0,100,25), 10, 10, Qt::AbsoluteSize);
    QPainterPath paintPath5050;
    paintPath5050.addRoundedRect(QRect(0,0,50,50), 0, 0, Qt::AbsoluteSize);

    QFont fontInit(QStringLiteral("Arial"), 12, 4);

    labelLong = QSharedPointer<QLabel>::create(this->parentWidget());
    labelLong->setAutoFillBackground(true);
    labelLong->setMask(paintPath150.toFillPolygon().toPolygon());
    labelLong->setAlignment(Qt::AlignCenter);
    labelLong->setContentsMargins(0, 0, 0, 25);
    labelLong->setFont(fontInit);

    labelLat = QSharedPointer<QLabel>::create(this->parentWidget());
    labelLat->setAutoFillBackground(true);
    labelLat->setAlignment(Qt::AlignCenter);
    labelLat->setMask(paintPath150.toFillPolygon().toPolygon());
    labelLat->setContentsMargins(0, 0, 0, 25);
    labelLat->setFont(fontInit);

    labelRadius = QSharedPointer<QLabel>::create(this->parentWidget());
    labelRadius->setAutoFillBackground(true);
    labelRadius->setAlignment(Qt::AlignCenter);
    labelRadius->setMask(paintPath100.toFillPolygon().toPolygon());
    labelRadius->setContentsMargins(0, 0, 0, 25);
    labelRadius->setFont(fontInit);

    labelAzim   = QSharedPointer<QLabel>::create(this->parentWidget());
    labelAzim->setAutoFillBackground(true);
    labelAzim->setAlignment(Qt::AlignCenter);
    labelAzim->setMask(paintPath100.toFillPolygon().toPolygon());
    labelAzim->setContentsMargins(0, 0, 0, 25);
    labelAzim->setFont(fontInit);

    //Отображение текущего времени
    QPainterPath paintPathTime;
    paintPathTime.addRoundedRect(QRect(0,0,250,25), 10, 10, Qt::AbsoluteSize);

    labelTime   = QSharedPointer<QLabel>::create(this->parentWidget());
    labelTime->setAutoFillBackground(true);
    labelTime->setAlignment(Qt::AlignCenter);
    labelTime->setMask(paintPathTime.toFillPolygon().toPolygon());
    labelTime->setContentsMargins(0, 0, 0, 25);
    labelTime->setFont(fontInit);

    //Сразу же создаем таймер для лямбды функции отображения времени, так в дальнейшем нет необходимости
    //Контролировать Label
    timerLbTime = QSharedPointer<QTimer>::create(this);
    connect(timerLbTime.data(), &QTimer::timeout, [=]()
    {
        labelTime->setText(QTime::currentTime().toString() + " " + QDate::currentDate().toString());
    });
    timerLbTime->start(500);

    //Создаем элементы управления картой
    //Кнопка измерения расстояния на карте(Линейка)
    butMeasure = QSharedPointer<ButtonSettings>::create(this->parentWidget());
    butMeasure->setIcon(QIcon(QPixmap(":/Icons/w_free-icon-font-ruler-triangle-7931122.png")));
    butMeasure->setToolTip(QStringLiteral("Измерение расстояния"));
    butMeasure->setMask(paintPath5050.toFillPolygon().toPolygon());

    if(!mapContr.isNull())
        mapMeas = mapContr->createMeasure();

    //Подключаемся
    connect(butMeasure.data(), &ButtonSettings::clicked, this, [=]()
    {
        stateMeasure ^=1;

        updateStateMove     (false);
        updateStateZoomPlus (false);
        updateStateZoomMinus(false);
        updateStateMeasure  (stateMeasure);
        if(mapMeas != nullptr)
            mapMeas->setState(stateMeasure);


        //Обновление линейки
        mapMeas->setState(false);

    });

    //Кнопка перемещения по карте
    butMove = QSharedPointer<ButtonSettings>::create(this->parentWidget());
    butMove->setIcon(QIcon(QPixmap(":/Icons/move_i.png")));
    butMove->setAutoFillBackground(true);
    butMove->setToolTip(QStringLiteral("Перемещение по карте"));
    butMove->setMask(paintPath5050.toFillPolygon().toPolygon());

    //Подключаемся
    connect(butMove.data(), &ButtonSettings::released, this, [=]()
    {
        stateMove ^=1;

        updateStateMeasure  (false);
        updateStateZoomPlus (false);
        updateStateZoomMinus(false);
        updateStateMove     (stateMove);

        //Обновление линейки
        if(mapMeas != nullptr)
            mapMeas->setState(false);
    });


    butZoomPlus = QSharedPointer<ButtonSettings>::create(this->parentWidget());
    butZoomPlus->setIcon(QIcon(QPixmap(":/Icons/w_free-icon-font-zoom-in-3917253.png")));
    butZoomPlus->setToolTip(QStringLiteral("Приближение карты"));
    butZoomPlus->setMask(paintPath5050.toFillPolygon().toPolygon());

    //Подключаемся
    connect(butZoomPlus.data(), &ButtonSettings::clicked, this, [=]()
    {
        stateZoomPlus ^=1;

        updateStateMeasure  (false);
        updateStateMove     (false);
        updateStateZoomMinus(false);
        updateStateZoomPlus (stateZoomPlus);

        //Обновление линейки
        if(mapMeas != nullptr)
            mapMeas->setState(false);
    });


    butZoomMinus = QSharedPointer<ButtonSettings>::create(this->parentWidget());
    butZoomMinus->setIcon(QIcon(QPixmap(":/Icons/w_free-icon-font-zoom-out-3917253.png")));
    butZoomMinus->setToolTip(QStringLiteral("Отдаление карты"));
    butZoomMinus->setMask(paintPath5050.toFillPolygon().toPolygon());

    //Подключаемся
    connect(butZoomMinus.data(), &ButtonSettings::clicked, this, [=]()
    {
        stateZoomMinus ^= 1;

        updateStateMeasure  (false);
        updateStateMove     (false);
        updateStateZoomPlus (false);
        updateStateZoomMinus(stateZoomMinus);

        //Обновление линейки
        if(mapMeas != nullptr)
            mapMeas->setState(false);
    });

    //Кнопка Включения/Отключения камеры
    butCamera = QSharedPointer<ButtonSettings>::create(this->parentWidget());
    butCamera->setIcon(QIcon(QPixmap(":/Icons/video-camera-outline_icon-icons.com_73112.png")));
    butCamera->setToolTip(QStringLiteral("Камера"));
    butCamera->setMask(paintPath5050.toFillPolygon().toPolygon());


    //Подключаемся
    connect(butCamera.data(), &ButtonSettings::clicked, this, [=]()
    {
        static bool state = false;
        state ^=1;
        if(state)
        {
            Q_EMIT setStateCamera(true);
            butCamera->setConstGreenPalette(true);
        }
        else
        {
            Q_EMIT setStateCamera(false);
            butCamera->setConstGreenPalette(false);
        }
    });

    //Кнопка Установок
    butSet = QSharedPointer<ButtonSettings>::create(this->parentWidget());
    butSet->setIcon(QIcon(QPixmap(":/Icons/w_settings_icon_251265.png")));
    butSet->setText(QStringLiteral("Установки"));
    butSet->setFont(fontInit);
    butSet->setToolTip(QStringLiteral("Установки"));

    //Подключаемся
    connect(butSet.data(), &ButtonSettings::clicked, this, [=]()
    {
        Q_EMIT setShowSettings(true);
    });

    //Кнопка журнала
    butList = QSharedPointer<ButtonSettings>::create(this->parentWidget());
    butList->setIcon(QIcon(QPixmap(":/Icons/w_free-icon-font-align-justify-3917034.png")));
    butList->setText(QStringLiteral(" Журнал"));
    butList->setFont(fontInit);
    butList->setToolTip(QStringLiteral("Журнал"));

    //Подключаемся
    connect(butList.data(), &ButtonSettings::clicked, this, [=]()
    {
        Q_EMIT setShowJournal(true);
    });
}


/** @brief Обновляем текущие географические координаты мыши */
void sceneControl::updateGeoCoord()
{
    if(!mapContr.isNull())
    {
        //Вычисляем широту и долготу координаты мыши на сцене
        QPointF point = QPointF(mapContr->qgs2ll(mapToScene(mapFromGlobal(QCursor::pos())), mapContr->zoomLevel()));

        switch (cordSystem)
        {
        case CoordinatSystem::WGS84GeoCentr:
        {
            static double x,y,z;

            calcul.WGS84GeodezToWGS84Geocentr(point.x(), point.y(), 0, x, y, z);
            labelLat->setText(QString::number(x));
            labelLong->setText(QString::number(y));
            break;
        }
        case CoordinatSystem::WGS84Geodez:
        {
            labelLat->setText(QString::number(point.y()));
            labelLong->setText(QString::number(point.x()));

            break;
        }
        case CoordinatSystem::WGS84GeoZone:
        {
            static int zone;
            static bool northp;
            static double x, y;
            GeographicLib::UTMUPS::Forward(point.y(), point.x(), zone, northp, x, y);

            labelLat->setText(QString::number(zone) + " Z " + QString::number(x,'f',1));
            labelLong->setText(QString::number(y,'f',1));
            break;
        }
        default:
            break;
        }
    }

    labelRadius->setText(QStringLiteral("Rм"));
    labelAzim->setText(QStringLiteral("aм"));
}

quint8 sceneControl::getZoomLevel() const
{
    if(!mapContr.isNull())
        return std::move(mapContr->zoomLevel());
    else return 0;
}

QSharedPointer<MapControl> sceneControl::getMapContr() const
{
    return mapContr;
}

/** @brief Получаем текущие координаты центра экрана в широте долготе WGS 84*/
QPointF sceneControl::getWGS84Center()
{
    if(!mapContr.isNull())
    {
        return mapContr->qgs2ll(mapToScene(rect().center()), mapContr->zoomLevel());
    }
    else
    {
        qDebug() << "ERROR  sceneControl::getWGS84Center() !mapContr.isNull()";
        return QPointF(0,0);
    }
}

void sceneControl::wheelEvent(QWheelEvent *event)
{
    int steps = -event->angleDelta().y()/120;

    if(steps == 0)
    {
        event->ignore();
        return;
    }
    else if(steps > 1)  steps = 1;
    else if(steps < -1) steps = -1;

    //Новый зум
    quint8 zoom = mapContr->zoomLevel()-steps;
    if(zoom < 2) zoom = 2;
    else if(zoom > 20) zoom = 20;

    //Изменяем масштабрирование карты
    if(!mapContr.isNull())
    {
        mapContr->setZoomLevel(zoom, ZoomMode::MouseZoom);
    }

    event->accept();
}

void sceneControl::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    //Кастомная привязка к координатам экрана
    if(width() > 850)
    {
        if(!labelLat.isNull())     labelLat->setGeometry    (10,  height() - 40, 170, 50);
        if(!labelLong.isNull())    labelLong->setGeometry   (190, height() - 40, 170, 50);
        if(!labelRadius.isNull())  labelRadius->setGeometry (370, height() - 40, 100, 50);
        if(!labelAzim.isNull())    labelAzim->setGeometry   (480, height() - 40, 100, 50);
    }
    else if (width() <= 850)
    {
        if(!labelLat.isNull())     labelLat->setGeometry    (10, height() - 40 - 50*4, 170, 50);
        if(!labelLong.isNull())    labelLong->setGeometry   (10, height() - 40 - 50*3, 170, 50);
        if(!labelRadius.isNull())  labelRadius->setGeometry (10, height() - 40 - 50*2, 100, 50);
        if(!labelAzim.isNull())    labelAzim->setGeometry   (10, height() - 40 - 50  , 100, 50);
    }
    if(!labelTime.isNull())    labelTime->setGeometry   (width() - 260,  10, 250, 50);

    if(width() > 550)
    {
        if(!butZoomPlus.isNull())  butZoomPlus->setGeometry (width() - 320, 10, 50, 50);
        if(!butZoomMinus.isNull()) butZoomMinus->setGeometry(width() - 380, 10, 50, 50);
        if(!butMove.isNull())      butMove->setGeometry     (width() - 440, 10, 50, 50);
        if(!butMeasure.isNull())   butMeasure->setGeometry  (width() - 500, 10, 50, 50);
    }

    else if (width() <= 550)
    {
        if(!butZoomPlus.isNull())  butZoomPlus->setGeometry (width() - 60, 70, 50, 50);
        if(!butZoomMinus.isNull()) butZoomMinus->setGeometry(width() - 60, 130, 50, 50);
        if(!butMove.isNull())      butMove->setGeometry     (width() - 60, 190, 50, 50);
        if(!butMeasure.isNull())   butMeasure->setGeometry  (width() - 60, 250, 50, 50);
    }

    if(!butCamera.isNull())    butCamera->setGeometry   (10, 10, 50, 50);
    if(!butSet.isNull())       butSet->setGeometry      (width() - 160, height() - 60, 150, 50);
    if(!butList.isNull())      butList->setGeometry     (width() - 300, height() - 60, 130, 50);
}

void sceneControl::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    //emit sceneMove(event);)


    //Обновляем текущие географические координаты мыши
    updateGeoCoord();

}

void sceneControl::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    if(event->button() == Qt::LeftButton)
    {
        //emit scenePress(event);

        //Приближение
        if(stateZoomPlus)
        {
            //Изменяем масштабрирование карты
            if(!mapContr.isNull())
                if(!mapContr->tileSource().isNull())
                {
                    //Новый зум
                    quint8 zoom = mapContr->zoomLevel() + 1;
                    if(zoom < mapContr->tileSource()->minZoomLevel())       zoom = mapContr->tileSource()->minZoomLevel();
                    else if(zoom >  mapContr->tileSource()->maxZoomLevel()) zoom = mapContr->tileSource()->maxZoomLevel();
                    mapContr->setZoomLevel(zoom, ZoomMode::MouseZoom);
                }
        }
        //Отдаление
        if(stateZoomMinus)
        {
            //Изменяем масштабрирование карты
            if(!mapContr.isNull())
                if(!mapContr->tileSource().isNull())
                {
                    //Новый зум
                    quint8 zoom = mapContr->zoomLevel() - 1;
                    if(zoom < mapContr->tileSource()->minZoomLevel())       zoom = mapContr->tileSource()->minZoomLevel();
                    else if(zoom >  mapContr->tileSource()->maxZoomLevel()) zoom = mapContr->tileSource()->maxZoomLevel();
                    if(!mapContr.isNull()) mapContr->setZoomLevel(zoom, ZoomMode::MouseZoom);
                }
        }

        //Обновление линейки
        if(stateMeasure)
        {

            if(mapMeas != nullptr)
                mapMeas->mousePress(mapToScene(event->pos()));
        }
    }
    else if(event->button() == Qt::RightButton)
    {
        //Обновление линейки
        if(mapMeas != nullptr)
        {
            mapMeas->setState(false);
            mapMeas->setState(true);
        }

    }

    //Событие обработано, дальнейшая передача не нужна
    event->accept();
}

void sceneControl::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    //emit sceneRelease(event);

}

void sceneControl::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);

}

void sceneControl::keyPressEvent(QKeyEvent *evt)
{
    QGraphicsView::keyPressEvent(evt);
    int key = ((int)evt->key() | (int)evt->modifiers());

    switch(key)
    {
    case (Qt::Key_Q):
    {
        rotate(-10);
        break;
    }
    case (0x0419):
    {
        rotate(-10);
        break;
    }
    case (Qt::Key_E):
    {
        rotate(10);
        break;
    }
    case (0x0423):
    {
        rotate(10);
        break;

    }
    case (Qt::Key_W):
    {
        verticalScrollBar()->setValue(verticalScrollBar()->value() - 10);
        break;
    }
    case (0x0426):
    {
        verticalScrollBar()->setValue(verticalScrollBar()->value() - 10);
        break;
    }

    case (Qt::Key_A):
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 10);
        break;
    }
    case (0x0424):
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - 10);
        break;
    }

    case (Qt::Key_S):
    {
        verticalScrollBar()->setValue(verticalScrollBar()->value() + 10);
        break;
    }

    case (0x042b):
    {
        verticalScrollBar()->setValue(verticalScrollBar()->value() + 10);
        break;
    }

    case (Qt::Key_D):
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 10);
        break;
    }

    case (0x0412):
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + 10);
        break;
    }
    case (Qt::Key_1):
    {
        mapContr->setMapType(typeMaps::osmMaps);
        break;
    }
    case (Qt::Key_2):
    {
        mapContr->setMapType(typeMaps::googleGEOMaps);
        break;
    }
    case (Qt::Key_3):
    {
        mapContr->setMapType(typeMaps::googleMaps);
        break;
    }
    case (Qt::Key_4):
    {
        mapContr->setMapType(typeMaps::statmen);
        break;
    }
    case (Qt::Key_5):
    {
        mapContr->setMapType(typeMaps::openToppoMap);
        break;
    }

    default:
        break;
    }
}

void sceneControl::updateStateMeasure(bool newState)
{
    stateMeasure = newState;
    if(stateMeasure)
    {
        butMeasure->setConstGreenPalette(true);
        Q_EMIT setStateMeasure(true);
    }
    else
    {
        butMeasure->setConstGreenPalette(false);
        Q_EMIT setStateMeasure(false);
    }
}

void sceneControl::updateStateMove(bool newState)
{
    stateMove = newState;
    if(stateMove)
    {
        //Режим перетаскивания по умолчанию позволяет нам перетаскивать карту, чтобы перемещать вид.
        setDragMode(QGraphicsView::ScrollHandDrag);
        butMove->setConstGreenPalette(true);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        setDragMode(QGraphicsView::NoDrag);
        butMove->setConstGreenPalette(false);
    }
}

void sceneControl::updateStateZoomPlus(bool newState)
{
    stateZoomPlus = newState;
    if(stateZoomPlus)
    {
        setCursor(QPixmap(QStringLiteral(":/Icons/free-icon-font-zoom-in-3917253.png")).scaled(30,30, Qt::KeepAspectRatio));
        butZoomPlus->setConstGreenPalette(true);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        butZoomPlus->setConstGreenPalette(false);
    }
}

void sceneControl::updateStateZoomMinus(bool newState)
{
    stateZoomMinus = newState;
    if(stateZoomMinus)
    {
        setCursor(QPixmap(QStringLiteral(":/Icons/free-icon-font-zoom-out-3917253.png")).scaled(30,30, Qt::KeepAspectRatio));
        butZoomMinus->setConstGreenPalette(true);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
        butZoomMinus->setConstGreenPalette(false);
    }
}
