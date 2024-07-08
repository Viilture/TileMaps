#include "videocontrol.h"

videoControl::videoControl(QWidget *parent, sceneControl* newScenControl) : QStringListModel(parent),
    scenControl(newScenControl)
{
    /** @brief Управление сигналами */
    manager = managerSignals::create();
    manager->addVideoControl(this);

    //Считываем настройки Ini Файла, если они есть
    set = QSharedPointer<SettingsBR>::create();

    init();

    rememberAllCameras();

    // insertRow(2);
    // setData(QModelIndex(0,0), QVariant("data"));

}

videoControl::~videoControl()
{

}

videoWidget *videoControl::getCameraWid() const
{
    return cameraWid;
}

/** @brief Восстановим все камеры из ini файла */
void videoControl::rememberAllCameras()
{
    if(set.isNull()) return;

    //Количество камер
    int32_t sizeCamera = set->value(QStringLiteral("Cameras/Camera/size"), 0).toUInt();

    for (int i = 0; i < sizeCamera; i++)
    {
        QString key = "Cameras/Camera" + QString::number(i + 1) + "/";

        double lat           = set->value(key + "lat", 0.0).toDouble();
        double longit        = set->value(key + "long", 0.0).toDouble();
        double azim          = set->value(key + "azim", 0.0).toDouble();
        double tang          = set->value(key + "tang", 0.0).toDouble();
        double kren          = set->value(key + "kren", 0.0).toDouble();
        QHostAddress ip4     = QHostAddress(set->value(key + "ip4", 0.0).toString());
        QString name         = set->value(key + "name", "Camera").toString();

        createCamera(lat, longit, azim, tang, kren, ip4, name);
    }
}


void videoControl::init()
{
    qRegisterMetaType<QSharedPointer<QPixmap>>("QSharedPointer<QPixmap>");
    qRegisterMetaType<int32_t>("int32_t");
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<QHostAddress>("QHostAddress");

    cameraWid = new videoWidget();

    //vidExchange = QSharedPointer<VideoExchange>::create(this, 0, modeExchange::client, 9999);
}

/** @brief Создаем новую камеру */
void videoControl::createNewCamera()
{
    try {

        if(set.isNull()) return;

        beginResetModel();

        //Не более 32 камер
        if(listCameras.size() >= 32) { qDebug() << "Создание Камеры невозможно - Ограничение 32 камеры"; return;}

        QSharedPointer<CameraControl> camera = QSharedPointer<CameraControl>::create(this, scenControl);
        if(!camera.isNull())
        {
            listCameras.push_back(camera);

            //Устанавливаем имя
            camera->setName("Camera " + QString::number(listCameras.size()));

            //И сохраняем все настройки в ini файл
            set->setValue(QStringLiteral("Cameras/camera/size"),   listCameras.size());

            QString key = "Cameras/camera" + QString::number(listCameras.size()) + "/";

            set->setValue(key + "name",   camera->getName());
            set->setValue(key + "lat",    camera->getCoordWGS84().x());
            set->setValue(key + "long",   camera->getCoordWGS84().y());
            set->setValue(key + "azim",   camera->getAzim());
            set->setValue(key + "tang",   camera->getTang());
            set->setValue(key + "kren",   camera->getKren());
            set->setValue(key + "ip4",    camera->getIp4().toString());

        }
        else
        {
            qDebug() << "Error Create new Camera";
        }

        endResetModel();

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::createNewCamera()";
    }
}

/** @brief Создание камеры */
void videoControl::createCamera(double lat, double longit, double azim, double tang, double kren, QHostAddress ip4, QString nameRli)
{
    if(set.isNull()) return;

    try {
        beginResetModel();

        //Не более 32 Радаров
        if(listCameras.size() >= 32) { qDebug() << ">32 Camera"; return;}

        if(!ip4.isGlobal() && !ip4.isLinkLocal()) ip4.setAddress("127.0.0.1");
        if(lat > 90 || lat < -90) lat = 0;
        if(longit > 180 || longit < -180) longit = 0;
        if(azim > 360 || azim < 0) azim = 0.0;
        if(tang > 360 || tang < 0) tang = 0.0;
        if(kren > 360 || kren < 0) kren = 0.0;
        if(nameRli.isNull() || nameRli.isEmpty()) nameRli = "CameraE";

        QSharedPointer<CameraControl> camera = QSharedPointer<CameraControl>::create(this, scenControl);
        if(!camera.isNull())
        {
            camera->setLat(lat);
            camera->setLong(longit);
            camera->setAzim(azim);
            camera->setTang(tang);
            camera->setKren(kren);
            camera->setIp4(ip4);
            camera->setName(nameRli);

            listCameras.push_back(camera);

            //И сохраняем все настройки в ini файл
            set->setValue(QStringLiteral("Cameras/camera/size"),   listCameras.size());

            QString key = "Cameras/camera" + QString::number(listCameras.size()) + "/";

            set->setValue(key + "lat",    camera->getCoordWGS84().x());
            set->setValue(key + "long",   camera->getCoordWGS84().y());
            set->setValue(key + "azim",   camera->getAzim());
            set->setValue(key + "tang",   camera->getTang());
            set->setValue(key + "kren",   camera->getKren());
            set->setValue(key + "ip4",    camera->getIp4().toString());
            set->setValue(key + "name",   camera->getName());

        }
        else
        {
            qDebug() << "Error Create new Camera";
        }

        endResetModel();

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::createCamera()";
    }
}

/** @brief Удаление камееры */
void videoControl::removeCamera(int32_t numCamera)
{
    beginResetModel();

    try {

        if((numCamera < 1) || (numCamera > listCameras.size())) return;

        listCameras.removeAt(numCamera - 1);

        saveToIni();

        endResetModel();

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::removeCamera()";
    }
}

/** @brief Сохраняем данные в Ini файл */
void videoControl::saveToIni()
{
    if(set.isNull()) return;

    try {

        set->remove(QStringLiteral("Cameras"));

        set->setValue(QStringLiteral("Cameras/camera/size"), listCameras.size());

        for(int i = 0; i < listCameras.size(); i++)
        {
            QString key = "Cameras/camera" + QString::number(i + 1) + "/";

            set->setValue(key + "lat",     listCameras.at(i)->getCoordWGS84().x());
            set->setValue(key + "long",    listCameras.at(i)->getCoordWGS84().y());
            set->setValue(key + "azim",    listCameras.at(i)->getAzim());
            set->setValue(key + "tang",    listCameras.at(i)->getTang());
            set->setValue(key + "kren",    listCameras.at(i)->getKren());
            set->setValue(key + "ip4",     listCameras.at(i)->getIp4().toString());
            set->setValue(key + "name",    listCameras.at(i)->getName());
        }

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::saveToIni()";
    }
}

/** @brief Получение IP4 */
QHostAddress videoControl::getIp4(int32_t numCamera)
{
    try {

        if((numCamera < 1) || (numCamera > listCameras.size())) { qDebug() << "Error" + QString::number(numCamera) + " not RLI"; return QHostAddress::Any;}
        return listCameras.at(numCamera - 1)->getIp4();

    }  catch (...)
    {
        qDebug() << "Exception -  videoControl::getIp4";
    }

}

/** @brief Установка IP4 */
void videoControl::setIp4(int32_t numCamera, QHostAddress ip4)
{
    try {

        if(set.isNull()) return;
        if((numCamera < 1) || (numCamera > listCameras.size())) return;

        listCameras.at(numCamera - 1)->setIp4(ip4);

        //И Сохраняем все в ini файле
        QString key = "Cameras/camera" + QString::number(numCamera) + "/";
        set->setValue(key + "ip4", ip4.toString());

    }  catch (...)
    {
        qDebug() << "Exception -  videoControl::getIp4";
    }
}

double videoControl::getAzim(int32_t numCamera) const
{
    try {

        if((numCamera < 1) || (numCamera > listCameras.size())) return -1000;
        return listCameras.at(numCamera - 1)->getAzim();

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::getAzim";
    }

}

void videoControl::setAzim(int32_t numCamera, double newAzim)
{
    try {

        if(set.isNull()) return;
        if((newAzim > 360) || (newAzim < 0)) return;
        if((numCamera < 1) || (numCamera > listCameras.size())) return;

        listCameras.at(numCamera - 1)->setAzim(newAzim);

        //И Сохраняем все в ini файле
        QString key = "Cameras/camera" + QString::number(numCamera) + "/";
        set->setValue(key + "azim",   newAzim);

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::setAzim";
    }
}

double videoControl::getTang(int32_t numCamera) const
{
    try {

        if((numCamera < 1) || (numCamera > listCameras.size())) return -1000;
        return listCameras.at(numCamera - 1)->getTang();


    }  catch (...)
    {
        qDebug() << "Exception -  videoControl::getTang";
    }

}

void videoControl::setTang(int32_t numCamera, double newTang)
{
    try {

        if(set.isNull()) return;
        if((newTang > 360) || (newTang < 0)) return;
        if((numCamera < 1) || (numCamera > listCameras.size())) return;

        listCameras.at(numCamera - 1)->setTang(newTang);

        //И Сохраняем все в ini файле
        QString key = "Cameras/camera" + QString::number(numCamera) + "/";
        set->setValue(key + "tang",   newTang);

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::setTang";
    }
}

double videoControl::getKren(int32_t numCamera) const
{
    try {

        if((numCamera < 1) || (numCamera > listCameras.size())) return -1000;
        return listCameras.at(numCamera - 1)->getKren();

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::getKren";
    }
}

void videoControl::setKren(int32_t numCamera, double newKren)
{
    try {

        if(set.isNull()) return;
        if((newKren > 360) || (newKren < 0)) return;
        if((numCamera < 1) || (numCamera > listCameras.size())) return;

        listCameras.at(numCamera - 1)->setKren(newKren);

        //И Сохраняем все в ini файле
        QString key = "Cameras/camera" + QString::number(numCamera) + "/";
        set->setValue(key + "kren",   newKren);

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::setKren";
    }
}

double videoControl::getLat(int32_t numCamera) const
{
    try {

        if((numCamera < 1) || (numCamera > listCameras.size())) return -1000;
        return listCameras.at(numCamera - 1)->getCoordWGS84().x();

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::getLat";
    }

}

void videoControl::setLat(int32_t numCamera, double newLat)
{
    try {

        if(set.isNull()) return;
        if((newLat > 90) || (newLat < -90)) return;
        if((numCamera < 1) || (numCamera > listCameras.size())) return;

        listCameras.at(numCamera - 1)->setLat(newLat);

        //И Сохраняем все в ini файле
        QString key = "Cameras/camera" + QString::number(numCamera) + "/";
        set->setValue(key + "lat", newLat);

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::setLat";
    }
}

double videoControl::getLong(int32_t numCamera) const
{
    try {

        if((numCamera < 1) || (numCamera > listCameras.size())) return -1000;

        return listCameras.at(numCamera - 1)->getCoordWGS84().y();

    }  catch (...)
    {
        qDebug() << "videoControl::getLong";
    }
}

void videoControl::setLong(int32_t numCamera, double newLong)
{
    try {

        if(set.isNull()) return;
        if((numCamera < 1) || (newLong > 180) || (newLong < -180)) return;
        if((numCamera < 1) || (numCamera > listCameras.size())) return;

        listCameras.at(numCamera - 1)->setLong(newLong);

        //И Сохраняем все в ini файле
        QString key = "Cameras/camera" + QString::number(numCamera) + "/";
        set->setValue(key + "long", newLong);

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::setLong";
    }
}

QString videoControl::getName(int32_t numCamera) const
{
    try {
        if((numCamera < 1) || (numCamera > listCameras.size())) return QLatin1String("");
        return listCameras.at(numCamera - 1)->getName();
    }  catch (...)
    {
        qDebug() << "Exception - videoControl::getName";
    }
}

void videoControl::setName(int32_t numCamera, QString newName)
{
    try {

        if(set.isNull()) return;
        if((numCamera < 1) || (numCamera > listCameras.size())) return;

        listCameras.at(numCamera - 1)->setName(newName);

        //И Сохраняем все в ini файле
        QString key = "Cameras/camera" + QString::number(numCamera) + "/";
        set->setValue(key + "name", newName);

    }  catch (...)
    {
        qDebug() << "Exception -  videoControl::setName";
    }
}

int32_t videoControl::getCameraSize() const
{
    return listCameras.size();

}

/** @brief Выбрать Радар */
void videoControl::selectCamera(int32_t numCamera)
{
    try {
        if((numCamera < 1) || (numCamera > listCameras.size())) return;

        //Перемещаемся к радару на карте
        listCameras.at(numCamera - 1)->centerMap();

        //Мигаем радаром
        listCameras.at(numCamera - 1)->blinked();

        beginResetModel();
        selectedCamera = numCamera - 1;
        endResetModel();

        //Устанавливаем новый источник воспроизведения видео
        if(cameraWid != nullptr)
            cameraWid->setNewSourceVideo(listCameras.at(numCamera - 1)->getIp4());

    }  catch (...)
    {
        qDebug() << "Exception - videoControl::selectCamera";
    }
}

int videoControl::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return listCameras.size();
}

QVariant videoControl::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= listCameras.size() || index.row() < 0)
        return QVariant();

    switch(role)
    {
    case Qt::FontRole:
    {
        QFont font;
        font.setBold(false);
        font.setPointSize(20);
        font.setItalic(true);
        return font;
    }

    case Qt::TextAlignmentRole:
        return Qt::AlignLeft;

    case Qt::EditRole:
        return QString::number(2);
        break;

    case Qt::ForegroundRole:

        return QColor(Qt::white);

        break;

    case Qt::DisplayRole:

        if(index.row() < listCameras.size())
        {
            DataCameraItem data;
            data.name  = listCameras.at(index.row())->getName();

            if(selectedCamera == index.row())
                data.selectItem = 1;
            else
                data.selectItem = 0;

            data.connect = 1;

            return QVariant::fromValue(data);

        }
        else
            return QVariant();

        break;
    }
    return QVariant();
}
