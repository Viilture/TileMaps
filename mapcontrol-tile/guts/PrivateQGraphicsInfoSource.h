#ifndef PRIVATEQGRAPHICSINFOSOURCE_H
#define PRIVATEQGRAPHICSINFOSOURCE_H

#include <QSharedPointer>

#include "MapTileSource.h"

/*!
 \brief Этот абстрактный класс унаследован MapGraphicsView как реализация
        шаблона проектирования "инверсия зависимостей", или, по крайней мере, настолько хорошо, насколько я его помню.

        По сути, Private QGraphicsObject и PrivateQGraphicsScene нуждаются в информации из
        MapGraphicsView, но MapGraphicsView находится выше этих двух классов.

        Заставляя все ссылки на MapGraphicsView в PrivateQGraphicsObject и PrivateQGraphicsScene
        проходить через этот интерфейс, мы как бы устраняем восходящую зависимость во время компиляции.
*/
class PrivateQGraphicsInfoSource
{
public:
	PrivateQGraphicsInfoSource() = default;
public:
    virtual quint8 zoomLevel() const=0;

    virtual QSharedPointer<MapTileSource> tileSource() const=0;
	Q_DISABLE_COPY(PrivateQGraphicsInfoSource)
};

#endif // PRIVATEQGRAPHICSINFOSOURCE_H
