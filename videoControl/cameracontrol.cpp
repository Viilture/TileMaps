#include "cameracontrol.h"

CameraControl::CameraControl(QObject *parent, sceneControl* newScenControl) : QObject(parent),
    scenControl(newScenControl)
{
    createViewCamera();
}

CameraControl::~CameraControl()
{
    if((scenControl != nullptr) && (!scenControl->getMapContr().isNull()))
        scenControl->getMapContr()->removeCamera(viewCamera);
}

void CameraControl::createViewCamera()
{
    if(scenControl == nullptr)
    {
        qDebug() << "ERROR CameraControl::createViewCamera() scenControl == nullptr";
        return;
    }

    //Получаем широту долготу поля видимости
    coordWGS84 =  scenControl->getWGS84Center();
    //Получаем указатель на класс графического отображения Камеры
    viewCamera = scenControl->getMapContr()->createCamera(coordWGS84);
}

/** @brief центрировать карту */
void CameraControl::centerMap()
{
    if((scenControl != nullptr) && (!scenControl->getMapContr().isNull()))
        scenControl->getMapContr()->centerOn(coordWGS84.x(), coordWGS84.y());

}

/** @brief Помигать радаром */
void CameraControl::blinked()
{
    static int it;
    static QTimer timer;
    it = 0;

    connect(&timer, &QTimer::timeout, this, [=]()
    {
        if(viewCamera.isNull())
        {
            timer.stop();
            timer.disconnect(this);
            return;
        }

        it++;

        if(it > 30)
        {
            viewCamera->setOpacity(1);
            timer.stop();
            timer.disconnect(this);
            return;
        }
        else
        {
            viewCamera->setOpacity(0.5 + sin(it));

        }
    });

    timer.start(50);
}

/** @brief Создаем визуализацию радара на сцене */
QPointF CameraControl::getCoordWGS84() const
{
    return coordWGS84;
}

void CameraControl::setCoordWGS84(QPointF newCoordWGS84)
{
    coordWGS84 = newCoordWGS84;
}
double CameraControl::getAzim() const
{
    return azim;
}

void CameraControl::setAzim(double newAzim)
{
    if(newAzim > 360 || newAzim < 0) return;

    azim = newAzim;
    if(!viewCamera.isNull())
    {
        viewCamera->setAzim(newAzim);
    }
}

double CameraControl::getTang() const
{
    return tang;
}

void CameraControl::setTang(double newTang)
{
    tang = newTang;
}

double CameraControl::getKren() const
{
    return kren;
}

void CameraControl::setKren(double newKren)
{
    kren = newKren;
}

const QHostAddress &CameraControl::getIp4() const
{
    return ip4;
}

void CameraControl::setIp4(const QHostAddress &newIp4)
{
    ip4 = newIp4;
}

void CameraControl::setLat(double newLat)
{
    coordWGS84.setX(newLat);

    //Обновляем координаты положения Радара
    if(!viewCamera.isNull())
    {
        viewCamera->setCenterP(coordWGS84);
    }
}

void CameraControl::setLong(double newLong)
{
    coordWGS84.setY(newLong);

    //Обновляем координаты положения Радара
    if(!viewCamera.isNull())
    {
        viewCamera->setCenterP(coordWGS84);
    }
}

const QString &CameraControl::getName() const
{
    return name;
}

void CameraControl::setName(const QString &newName)
{
    name = newName;
}
