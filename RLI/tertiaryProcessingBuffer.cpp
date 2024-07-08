#include "tertiaryProcessingBuffer.h"

TertiaryProcessingBuffer::TertiaryProcessingBuffer(QObject *parent)
    : QAbstractTableModel(parent)
{
    init();
}

TertiaryProcessingBuffer::~TertiaryProcessingBuffer()
{

}

void TertiaryProcessingBuffer::init()
{
    //    //Создаем классы упраления графической составляющей цели
    //    for(auto it:scenControl)
    //    {
    //        if(it == nullptr) continue;
    //        QSharedPointer<MapControl> mapContr = it->getMapContr();
    //        if(mapContr.isNull()) continue;

    //        QSharedPointer<TargetControl> trgContrl = mapContr->createTargetControl();
    //        if(trgContrl.isNull())
    //        {qDebug() << "Error - createTargetControl() void RliBuffer::init()"; continue;}

    //        targContrls.push_back(mapContr->createTargetControl());
    //    }
}

/** @brief Добавляем цель **/
void TertiaryProcessingBuffer::pushTarget(QPointF target)
{
    //    vectPoints.push_back(target);
    //    for(auto it:targContrls)
    //    {
    //        it->addTarget();
    //    }
}

/** @brief Заменяем все цели **/
void TertiaryProcessingBuffer::updateAllTargets(QVector<QPointF> newVectPoints)
{
    //    vectPoints = newVectPoints;
    //    for(auto it:targContrls)
    //    {

    //    }
}

void TertiaryProcessingBuffer::setMapContr(QSharedPointer<MapControl> newMapContr)
{
    shMapContr = newMapContr;
}

int TertiaryProcessingBuffer::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 10;
}

int TertiaryProcessingBuffer::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 8;
}

QVariant TertiaryProcessingBuffer::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch(role)
    {
    case Qt::FontRole:
    {
        QFont font;
        font.setBold(false);
        font.setPointSize(16);
        font.setItalic(false);
        return font;
    }

    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;

    case Qt::EditRole:
        return QString::number(2);
        break;

    case Qt::ForegroundRole:
        return QColor(Qt::white);
        break;

    case Qt::DisplayRole:
    {

        switch (index.column())
        {
        case 0:
        {
            return index.row();
            break;
        }

        case 1:
        {
            return  QRandomGenerator::global()->bounded(0, 12);
            break;
        }
        case 2:
        {
            int type = QRandomGenerator::global()->bounded(1, 3);

            switch (type)
            {
            case 1: {return  "Машина"; break;}
            case 2: {return  "Танк"; break;}
            case 3: {return  "Дрон"; break;}
            default: break;
            }
            break;
        }

        case 3:
        {
            int daln = QRandomGenerator::global()->bounded(1, 2500);

            return  QString::number(daln) + " м";

            break;
        }

        case 4:
        {
            int speed = QRandomGenerator::global()->bounded(1, 1000);

            return  QString::number(speed) + " м/с";

            break;
        }

        case 5:
        {
            int az = QRandomGenerator::global()->bounded(1, 360);

            return  QString::number(az);

            break;
        }

        case 6:
        {
            return  QTime::currentTime().toString();

            break;
        }

        case 7:
        {

            return  "Да";

            break;
        }


        default:
        {
            return QVariant();
            break;
        }
        }

        break;
    }
    default:
    {
        return QVariant();
        break;
    }
    }
    return QVariant();
}

QVariant TertiaryProcessingBuffer::headerData(int section, Qt::Orientation orientation, int role) const
{

    switch(role)
    {
    case Qt::FontRole:
    {
        QFont font;
        font.setBold(true);
        font.setPointSize(16);
        font.setItalic(false);
        return font;
    }

    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;

    case Qt::EditRole:
        return QString::number(2);
        break;

    case Qt::ForegroundRole:
        return QColor(Qt::white);
        break;

    case Qt::DisplayRole:
    {

        switch (section)
        {
        case 0:
        {

            return "№ п/п";
            break;
        }

        case 1:
        {

            return "№ РЛУ";
            break;
        }
        case 2:
        {

            return "Тип";
            break;
        }
        case 3:
        {

            return "Дальность";
            break;
        }
        case 4:
        {

            return "Скорость";
            break;
        }
        case 5:
        {

            return "Азимут";
            break;
        }
        case 6:
        {

            return "Время посл. обнаруж.";
            break;
        }
        case 7:
        {

            return "Обнаружение (да/нет)";
            break;
        }
        default:
        {
            return QVariant();
            break;
        }
        }

        break;
    }
    default:
    {
        return QVariant();
        break;
    }
    }


}
