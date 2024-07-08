#include "vidcameradelegate.h"

vidCameraDelegate::vidCameraDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

    textOption.setWrapMode(QTextOption::WordWrap);
}

vidCameraDelegate::~vidCameraDelegate()
{

}

void vidCameraDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    static DataCameraItem data;

    if(!index.data().canConvert<DataCameraItem>()) return;

    painter->save();

    static QPen penorder(Qt::white);
    static QPen pen(Qt::black);
    static QFont fontOrder = QFont("Arial", 14);

    //Получаем структуру с данными
    data = index.data().value<DataCameraItem>();

    //Отрисовываем черный фон для выбранных итемов
    if(data.selectItem || option.state&QStyle::State_HasFocus)
    {
        painter->fillRect(option.rect, Qt::black);
    }


    //Отрисовываем текст
    painter->setFont(fontOrder);
    painter->setPen(penorder);
    painter->drawText(option.rect.adjusted(40,8,0,0), data.name, textOption);

    //Отрисовываем иконку
    if(data.connect)
        painter->drawPixmap(QRect(option.rect.x()+5, option.rect.y()+5, 30, 30), QPixmap(":/Icons/g_video-camera-outline_icon-icons.com_73112.png"));
    else
        painter->drawPixmap(QRect(option.rect.x()+5, option.rect.y()+5, 30, 30), QPixmap(":/Icons/r_video-camera-outline_icon-icons.com_73112.png"));

    painter->restore();
}
