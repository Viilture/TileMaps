#ifndef GLOBALBARIER2_H
#define GLOBALBARIER2_H

#include <QString>
#include <QObject>


/** @struct Структура данных RLI view item для передачи через QStringListModel */
struct DataRliItem
{
    /** @brief имя РЛУ */
    QString name;
    /** @brief итем выбран */
    bool selectItem;
    /** @brief наличие связи */
    bool connect;
};

/** @struct Структура данных Camera view item для передачи через QStringListModel */
struct DataCameraItem
{
    /** @brief имя камеры */
    QString name;
    /** @brief итем выбран */
    bool selectItem;
    /** @brief наличие связи */
    bool connect;
};

Q_DECLARE_METATYPE(DataRliItem)
Q_DECLARE_METATYPE(DataCameraItem)

#endif // GLOBALBARIER2_H
