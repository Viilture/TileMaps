#ifndef RLIBUFFER_H
#define RLIBUFFER_H

#include <QObject>
#include <QSharedPointer>
#include <QByteArray>
#include <QList>
#include <QFile>
#include <QStandardPaths>
#include <QTimer>
#include <QTextStream>
#include <QIODevice>
#include <QDir>
#include <QDebug>
#include <QString>
#include <QTime>
#include <mutex>


class RLIbuffer : public QObject
{
    Q_OBJECT

    QSharedPointer<QTimer> timer2;

    QList<QSharedPointer<QByteArray>> arrayList;

public:
    explicit RLIbuffer(QObject *parent = nullptr);
    virtual ~RLIbuffer();

    std::mutex mutexExchange;

public Q_SLOTS:

    /** @brief Добавить данные для сохранения*/
    void appendDataForSave(QSharedPointer<QByteArray> data);

    /** @brief Записать данные в текстовый файл */
    void saveDataToFile();

};

#endif // RLIBUFFER_H
