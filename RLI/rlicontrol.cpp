#include "rlicontrol.h"

RLIControl::RLIControl(QObject *parent, sceneControl* newScenControl) : QStringListModel(parent),
    scenControl(newScenControl)
{
    /** @brief Управление сигналами */
    manager = managerSignals::create();
    manager->addRLIControl(this);

    //Считываем настройки Ini Файла, если они есть
    set = QSharedPointer<SettingsBR>::create();

    rememberAllRadars();
}

RLIControl::~RLIControl()
{

}

/** @brief Восстановим все радары из ini файла */
void RLIControl::rememberAllRadars()
{
    try {

        if(set.isNull()) return;

        //Количество Радаров
        int32_t sizeRLU = set->value(QStringLiteral("RLS/Radar/size"), 0).toUInt();

        for (int32_t i = 0; i < sizeRLU; i++)
        {
            QString key = "RLS/Radar" + QString::number(i + 1) + "/";

            double lat           = set->value(key + "lat", 0.0).toDouble();
            double longit        = set->value(key + "long", 0.0).toDouble();
            double azim          = set->value(key + "azim", 0.0).toDouble();
            double tang          = set->value(key + "tang", 0.0).toDouble();
            double kren          = set->value(key + "kren", 0.0).toDouble();
            QString ip4          = set->value(key + "ip4", 0.0).toString();
            modeRLI mode         = (modeRLI)(set->value(key + "mode", 0).toUInt());
            int32_t litera       = set->value(key + "litera", 0.0).toUInt();
            QString name         = set->value(key + "name", "RLI").toString();

            createRadar(lat, longit, azim, tang, kren, ip4, mode, litera, name);
        }

    }  catch (...)
    {
        qDebug() << "Exception - rememberAllRadars()";
    }
}

/** @brief Получение IP4 */
QString RLIControl::getIp4(int32_t numRLI)
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) { qDebug() << "Error" + QString::number(numRLI) + " not RLI"; return "127.0.0.1";}
        return listRadars.at(numRLI - 1)->getIp4();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::getIp4";
    }
}

/** @brief Установка IP4 */
void RLIControl::setIp4(int32_t numRLI, QString ip4)
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return;
        if(set.isNull()) return;

        listRadars.at(numRLI - 1)->setIp4(ip4);

        //И Сохраняем все в ini файле
        QString key = "RLS/Radar" + QString::number(numRLI) + "/";
        set->setValue(key + "ip4",    ip4);

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::setIp4";
    }
}

/** @brief Получение литеры */
int32_t RLIControl::getLitera(int32_t numRLI)
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return -1;
        return listRadars.at(numRLI - 1)->getLitera();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::getLitera";
    }
}

/** @brief Установка литеры */
void RLIControl::setLitera(int32_t numRLI, int32_t litera)
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size()) || (litera > 4) || (litera < 1)) return;
        if(set.isNull()) return;

        listRadars.at(numRLI - 1)->setLitera(litera);

        //И Сохраняем все в ini файле
        QString key = "RLS/Radar" + QString::number(numRLI) + "/";
        set->setValue(key + "litera", listRadars.at(numRLI - 1)->getLitera());

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::setLitera";
    }
}

/** @brief Получение режима работы */
modeRLI RLIControl::getMode(int32_t numRLI)
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return modeRLI::air;
        return listRadars.at(numRLI - 1)->getMode();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::getMode";
    }
}

/** @brief Установка режима работы */
void RLIControl::setMode(int32_t numRLI, modeRLI mode)
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return;
        if(set.isNull()) return;

        listRadars.at(numRLI - 1)->setMode(mode);

        //И Сохраняем все в ini файле
        QString key = "RLS/Radar" + QString::number(numRLI) + "/";
        set->setValue(key + "mode",   mode);

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::setMode";
    }
}

double RLIControl::getAzim(int32_t numRLI) const
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return -1000;
        return listRadars.at(numRLI - 1)->getAzim();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::getAzim";
    }
}

void RLIControl::setAzim(int32_t numRLI, double newAzim)
{
    try {

        if((newAzim > 360) || (newAzim < 0)) return;
        if((numRLI < 1) || (numRLI > listRadars.size())) return;
        if(set.isNull()) return;

        listRadars.at(numRLI - 1)->setAzim(newAzim);

        //И Сохраняем все в ini файле
        QString key = "RLS/Radar" + QString::number(numRLI) + "/";
        set->setValue(key + "azim",   newAzim);

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::setAzim";
    }
}

double RLIControl::getTang(int32_t numRLI) const
{
    try {

        if((numRLI < 1) || (numRLI > static_cast<int32_t>(listRadars.size()))) return -1000;
        return listRadars.at(numRLI - 1)->getTang();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::getTang";
    }
}

void RLIControl::setTang(int32_t numRLI, double newTang)
{
    try {

        if((newTang > 360) || (newTang < 0)) return;
        if((numRLI < 1) || (numRLI > listRadars.size())) return;
        if(set.isNull()) return;

        listRadars.at(numRLI - 1)->setTang(newTang);

        //И Сохраняем все в ini файле
        QString key = "RLS/Radar" + QString::number(numRLI) + "/";
        set->setValue(key + "tang",   newTang);

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::setTang";
    }
}

double RLIControl::getKren(int32_t numRLI) const
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return -1000;
        return listRadars.at(numRLI - 1)->getKren();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::getKren";
    }
}

void RLIControl::setKren(int32_t numRLI, double newKren)
{
    try {

        if((newKren > 360) || (newKren < 0)) return;
        if((numRLI < 1) || (numRLI > listRadars.size())) return;
        if(set.isNull()) return;

        listRadars.at(numRLI - 1)->setKren(newKren);

        //И Сохраняем все в ini файле
        QString key = "RLS/Radar" + QString::number(numRLI) + "/";
        set->setValue(key + "kren",   newKren);

    }  catch (...)
    {
        qDebug() << "Exception -  RLIControl::setKren";
    }
}

double RLIControl::getLat(int32_t numRLI) const
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return -1000;
        return listRadars.at(numRLI - 1)->getCoordWGS84().x();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::getLat";
    }
}

void RLIControl::setLat(int32_t numRLI, double newLat)
{
    try {

        if((newLat > 180) || (newLat < -180)) return;
        if((numRLI < 1) || (numRLI > listRadars.size())) return;
        if(set.isNull()) return;

        listRadars.at(numRLI - 1)->setLat(newLat);

        //И Сохраняем все в ini файле
        QString key = "RLS/Radar" + QString::number(numRLI) + "/";
        set->setValue(key + "lat", newLat);

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::setLat";
    }
}

double RLIControl::getLong(int32_t numRLI) const
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return -1000;
        return listRadars.at(numRLI - 1)->getCoordWGS84().y();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::getLong";
    }
}

void RLIControl::setLong(int32_t numRLI, double newLong)
{
    try {

        if((newLong > 180) || (newLong < -180)) return;
        if((numRLI < 1) || (numRLI > listRadars.size())) return;
        if(set.isNull()) return;

        listRadars.at(numRLI - 1)->setLong(newLong);

        //И Сохраняем все в ini файле
        QString key = "RLS/Radar" + QString::number(numRLI) + "/";
        set->setValue(key + "long", newLong);

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::setLong";
    }
}

QString RLIControl::getName(int32_t numRLI) const
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return QLatin1String("");
        return listRadars.at(numRLI - 1)->getName();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::getName";
    }
}

void RLIControl::setName(int32_t numRLI, QString newName)
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return;
        if(set.isNull()) return;

        listRadars.at(numRLI - 1)->setName(newName);

        //И Сохраняем все в ini файле
        QString key = "RLS/Radar" + QString::number(numRLI) + "/";
        set->setValue(key + "name", newName);

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::setName";
    }
}

int32_t RLIControl::getRadarSize()
{
    return listRadars.size();
}

/** @brief Создаем новый радар */
void RLIControl::createNewRadar()
{
    try {

        if(set.isNull()) return;

        beginResetModel();

        //Не более 32 Радаров
        if(listRadars.size() >= 32) { qDebug() << "Создание РЛУ невозможно - Ограничение 32 радара"; return;}

        beginInsertRows(QModelIndex(), listRadars.size() - 1, listRadars.size());

        QSharedPointer<RadarControl> radar = QSharedPointer<RadarControl>::create(this, scenControl);
        if(!radar.isNull())
        {
            listRadars.push_back(radar);

            //Устанавливаем имя
            radar->setName("Radar " + QString::number(listRadars.size()));

            //И сохраняем все настройки в ini файл
            set->setValue(QStringLiteral("RLS/Radar/size"),   listRadars.size());

            QString key = "RLS/Radar" + QString::number(listRadars.size()) + "/";

            set->setValue(key + "name",   radar->getName());
            set->setValue(key + "lat",    radar->getCoordWGS84().x());
            set->setValue(key + "long",   radar->getCoordWGS84().y());
            set->setValue(key + "azim",   radar->getAzim());
            set->setValue(key + "tang",   radar->getTang());
            set->setValue(key + "kren",   radar->getKren());
            set->setValue(key + "ip4",    radar->getIp4());
            set->setValue(key + "mode",   radar->getMode());
            set->setValue(key + "litera", radar->getLitera());
        }
        else
        {
            qDebug() << "Error Create new Radar";
        }

        endResetModel();

    }  catch (...)
    {
        qDebug() << "RLIControl::createRadar1";
    }
}

/** @brief Создаем новый радар c заранее известными параметрами*/
void RLIControl::createRadar(double lat, double longit, double azim, double tang, double kren, QString ip4, modeRLI mode, int32_t litera, QString nameRli)
{
    try {

        if(set.isNull()) return;

        beginResetModel();


        //Не более 32 Радаров
        if(listRadars.size() >= 32)
        {
            return;
        }

        beginInsertRows(QModelIndex(), listRadars.size() - 1, listRadars.size());

        if(lat > 180 || lat < -180) lat = 0;
        if(longit > 180 || longit < -180) longit = 0;
        if(azim > 360 || azim < 0) azim = 0.0;
        if(tang > 360 || tang < 0) tang = 0.0;
        if(kren > 360 || kren < 0) kren = 0.0;
        if(mode > 1 || mode < 1) mode = modeRLI::air;
        if(litera > 4 || litera < 1) litera = 1;

        QSharedPointer<RadarControl> radar = QSharedPointer<RadarControl>::create(this, scenControl);
        if(!radar.isNull())
        {

            radar->setLat(lat);
            radar->setLong(longit);
            radar->setAzim(azim);
            radar->setTang(tang);
            radar->setKren(kren);
            radar->setIp4(ip4);
            radar->setMode(mode);
            radar->setLitera(litera);
            radar->setName(nameRli);

            listRadars.push_back(radar);
            //И сохраняем все настройки в ini файл

            set->setValue(QStringLiteral("RLS/Radar/size"),   listRadars.size());

            QString key = "RLS/Radar" + QString::number(listRadars.size()) + "/";

            set->setValue(key + "lat",    radar->getCoordWGS84().x());
            set->setValue(key + "long",   radar->getCoordWGS84().y());
            set->setValue(key + "azim",   radar->getAzim());
            set->setValue(key + "tang",   radar->getTang());
            set->setValue(key + "kren",   radar->getKren());
            set->setValue(key + "ip4",    radar->getIp4());
            set->setValue(key + "mode",   radar->getMode());
            set->setValue(key + "litera", radar->getLitera());
            set->setValue(key + "name",   radar->getName());

        }
        else
        {
            qDebug() << "Error Create new Radar";
        }

        endResetModel();

    }  catch (...)
    {
        qDebug() << "RLIControl::createRadar2";
    }
}

/** @brief Удаляем радар */
void RLIControl::removeRadar(int32_t numRLI)
{
    try {

        beginResetModel();

        if((numRLI < 1) || (numRLI > listRadars.size())) return;

        listRadars.removeAt(numRLI - 1);
        saveToIni();

        endResetModel();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::removeRadar()";
    }
}

/** @brief Сохраняем данные в Ini файл */
void RLIControl::saveToIni()
{
    if(set.isNull()) return;

    set->remove(QStringLiteral("RLS"));

    set->setValue(QStringLiteral("RLS/Radar/size"), listRadars.size());

    for(int i = 0; i < listRadars.size(); i++)
    {
        QString key = "RLS/Radar" + QString::number(i + 1) + "/";

        set->setValue(key + "lat",     listRadars.at(i)->getCoordWGS84().x());
        set->setValue(key + "long",    listRadars.at(i)->getCoordWGS84().y());
        set->setValue(key + "azim",    listRadars.at(i)->getAzim());
        set->setValue(key + "tang",    listRadars.at(i)->getTang());
        set->setValue(key + "kren",    listRadars.at(i)->getKren());
        set->setValue(key + "ip4",     listRadars.at(i)->getIp4());
        set->setValue(key + "mode",    listRadars.at(i)->getMode());
        set->setValue(key + "litera",  listRadars.at(i)->getLitera());
        set->setValue(key + "name",    listRadars.at(i)->getName());
    }
}

/** @brief Выбрать Радар */
void RLIControl::selectRadar(int32_t numRLI)
{
    try {

        if((numRLI < 1) || (numRLI > listRadars.size())) return;

        //Перемещаемся к радару на карте
        listRadars.at(numRLI - 1)->centerMap();

        //Мигаем радаром
        listRadars.at(numRLI - 1)->blinked();

        beginResetModel();
        selectedRLI = numRLI - 1;
        endResetModel();

    }  catch (...)
    {
        qDebug() << "Exception - RLIControl::selectRadar()";
    }
}

int RLIControl::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return listRadars.size();
}

QVariant RLIControl::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= listRadars.size() || index.row() < 0)
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

        if(index.row() < listRadars.size())
        {
            DataRliItem data;
            data.name = listRadars.at(index.row())->getName();

            if(selectedRLI == index.row())
                data.selectItem = 1;
            else
                data.selectItem = 0;

            data.connect = 0;

            return QVariant::fromValue(data);
        }
        else
            return QVariant();

        break;
    }
    return QVariant();
}
