#include "radarcontrol.h"



RadarControl::RadarControl(QObject *parent, sceneControl* newScenControl) : QObject(parent),
    scenControl(newScenControl)
{
    init();
}

RadarControl::~RadarControl()
{
    threadExchange.quit();
    threadExchange.wait();

    if((scenControl != nullptr) && (!scenControl->getMapContr().isNull()))
        scenControl->getMapContr()->removeRadar(viewRLI);
}

void RadarControl::init()
{
    createViewRLI();

    /** @brief Сетевой обмен с РЛУ */
    BrExchange = QSharedPointer<Barier2Exchange>::create(nullptr, modeExchange::client, 49999, "127.0.0.1", 56, 789);

    connect(this, &RadarControl::setHost, BrExchange.data(), &Barier2Exchange::setHost);
    connect(this, &RadarControl::setPort, BrExchange.data(), &Barier2Exchange::setPort);

    connect(this, &RadarControl::sendStateRLU, BrExchange.data(), &Barier2Exchange::sendStateRLU);
    connect(this, &RadarControl::sendRLUData1, BrExchange.data(), &Barier2Exchange::sendRLUData1);

    connect(this, &RadarControl::sendCmdSetMode, BrExchange.data(), &Barier2Exchange::sendCmdSetMode);
    connect(this, &RadarControl::sendRegistrData, BrExchange.data(), &Barier2Exchange::sendRegistrData);

    BrExchange->moveToThread(&threadExchange);
    threadExchange.start();
}

const QString &RadarControl::getName() const
{
    return name;
}

void RadarControl::setName(const QString &newName)
{
    name = newName;
}

/** @brief центрировать карту */
void RadarControl::centerMap()
{
    if((scenControl != nullptr) && (!scenControl->getMapContr().isNull()))
        scenControl->getMapContr()->centerOn(coordWGS84.x(), coordWGS84.y());
}

/** @brief Помигать радаром */
void RadarControl::blinked()
{
    static int it;
    static QTimer timer;
    it = 0;

    connect(&timer, &QTimer::timeout, this, [=]()
    {
        if(viewRLI.isNull())
        {
            timer.stop();
            timer.disconnect(this);
            return;
        }

        it++;

        if(it > 30)
        {
            viewRLI->setOpacity(1);
            timer.stop();
            timer.disconnect(this);
            return;
        }
        else
        {
            viewRLI->setOpacity(0.5 + sin(it));

        }
    });

    timer.start(50);
}



void RadarControl::createViewRLI()
{
    if(scenControl == nullptr)
    {
        qDebug() << "ERROR RadarControl::createViewRLI() scenControl == nullptr";
        return;
    }

    //Получаем широту долготу поля видимости
    coordWGS84 =  scenControl->getWGS84Center();
    //Получаем указатель на класс графического отображения Радара
    viewRLI = scenControl->getMapContr()->createRadar(coordWGS84);
}

/** @brief Создаем визуализацию радара на сцене */
QPointF RadarControl::getCoordWGS84() const
{
    return coordWGS84;
}

void RadarControl::setCoordWGS84(QPointF newCoordWGS84)
{
    coordWGS84 = newCoordWGS84;
}
double RadarControl::getAzim() const
{
    return azim;
}

void RadarControl::setAzim(double newAzim)
{
    if(newAzim > 360 || newAzim < 0) return;

    azim = newAzim;
    if(!viewRLI.isNull())
    {
        viewRLI->setAzim(newAzim);
    }
}

double RadarControl::getTang() const
{
    return tang;
}

void RadarControl::setTang(double newTang)
{
    tang = newTang;
}

double RadarControl::getKren() const
{
    return kren;
}

void RadarControl::setKren(double newKren)
{
    kren = newKren;
}

const QString &RadarControl::getIp4() const
{
    return ip4;
}

void RadarControl::setIp4(const QString &newIp4)
{
    if(BrExchange.isNull()) return;
    ip4 = newIp4;

    Q_EMIT setHost(ip4);
}

modeRLI RadarControl::getMode() const
{
    return mode;
}

void RadarControl::setMode(modeRLI newMode)
{
    mode = newMode;
}

uint32_t RadarControl::getLitera() const
{
    return litera;
}

void RadarControl::setLitera(uint32_t newLitera)
{
    litera = newLitera;
}

void RadarControl::setLat(double newLat)
{
    coordWGS84.setX(newLat);

    //Обновляем координаты положения Радара
    if(!viewRLI.isNull())
    {
        viewRLI->setCenterP(coordWGS84);
    }
}

void RadarControl::setLong(double newLong)
{
    coordWGS84.setY(newLong);

    //Обновляем координаты положения Радара
    if(!viewRLI.isNull())
    {
        viewRLI->setCenterP(coordWGS84);
    }
}

