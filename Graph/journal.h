#ifndef JOURNAL_H
#define JOURNAL_H

#include <QWidget>
#include <QSharedPointer>
#include <QWeakPointer>

#include "tertiaryProcessingBuffer.h"

namespace Ui {
class Journal;
}

class TertiaryProcessingBuffer;

class Journal : public QWidget
{
    Q_OBJECT

    /** @brief Третичная обработка */
    QWeakPointer<TertiaryProcessingBuffer> wTertBuffer;

public:
    explicit Journal(QWidget *parent = nullptr);
    virtual ~Journal();

    void setShow(bool state);
    void setTableModel(QWeakPointer<TertiaryProcessingBuffer> wTertBuffer);

private:
    Ui::Journal *ui;
};

#endif // JOURNAL_H
