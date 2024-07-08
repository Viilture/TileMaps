#include "targetcharacteristics.h"
#include "ui_targetcharacteristics.h"

TargetCharacteristics::TargetCharacteristics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TargetCharacteristics)
{
    ui->setupUi(this);

    //Видимость поверх других окон
    setWindowFlags(this->windowFlags()|Qt::WindowStaysOnTopHint);

    setAutoFillBackground(false);
}

TargetCharacteristics::~TargetCharacteristics()
{
    delete ui;
}
