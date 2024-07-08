#include "journal.h"
#include "ui_journal.h"

Journal::Journal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Journal)
{
    ui->setupUi(this);

    this->setAutoFillBackground(false);
    hide();

    //Видимость поверх других окон
    setWindowFlags(this->windowFlags()| Qt::WindowStaysOnTopHint);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

Journal::~Journal()
{
    delete ui;
}

void Journal::setShow(bool state)
{
    if(state)
    {
        show();
        raise();
    }
    else
    {
        hide();
    }
}

void Journal::setTableModel(QWeakPointer<TertiaryProcessingBuffer> wTertBuffer)
{
   QWeakPointer<TertiaryProcessingBuffer> buffer =  wTertBuffer.toStrongRef();

   if(buffer.isNull()) return;

   ui->tableView->setModel(buffer.data());
}
