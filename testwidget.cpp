#include "testwidget.h"
#include "ui_testwidget.h"
#include "guts/Conversions.h"

#define Barier2INI "/Barier2INI"

testWidget::testWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::testWidget)
{
    ui->setupUi(this);

    //Считываем настройки Ini Файла, если они есть
    SettingsBR set;

    QString ip4  = set.value(QStringLiteral("Exchange/ip4"),  "127.0.0.1").toString();
    QString port = set.value(QStringLiteral("Exchange/port"), "56232")    .toString();

    /** @brief Внешний обмен */
    //exchangeBR = QSharedPointer<Barier2Exchange>::create(ip4, port);

    ui->pushButton_2->setConstGreenPalette(true);
    ui->pushButton_6->setConstGreenPalette(true);
    ui->pushButton_8->setConstGreenPalette(true);
    ui->pushButton_14->setConstGreenPalette(true);

    buf = QSharedPointer<RLIbuffer>::create(this);

    buf->moveToThread(&thread);
    thread.start();

//    connect(exchangeBR.data(), &Barier2Exchange::appendDataForSave, buf.data(), &RLIbuffer::appendDataForSave);
//    connect(this, &testWidget::setSaveTofile, exchangeBR.get(), &Barier2Exchange::setSaveTofile);

//    connect(exchangeBR.get(), &Barier2Exchange::updParam, this, &testWidget::updParam);

//    qRegisterMetaType<QSharedPointer<QByteArray>>("QSharedPointer<QByteArray>");
//    qRegisterMetaType<SendSettings>("SendSettings");
}

testWidget::~testWidget()
{
    thread.quit();
    thread.wait();
    delete ui;
}

void testWidget::on_pushButton_2_clicked()
{

    litera = 1;

    sendMessage();
}

void testWidget::on_pushButton_3_clicked()
{
    litera = 2;
    sendMessage();
}

void testWidget::on_pushButton_4_clicked()
{
    litera     = 3;
    sendMessage();
}


void testWidget::on_pushButton_5_clicked()
{

    litera     = 4;

    sendMessage();
}

void testWidget::on_pushButton_6_clicked()
{
    modulation     = 0;
    sendMessage();
}

void testWidget::on_pushButton_7_clicked()
{
    modulation     = 1;
    sendMessage();
}

void testWidget::on_pushButton_8_clicked()
{
    reciever     = 0;

    sendMessage();
}

void testWidget::on_pushButton_9_clicked()
{
    reciever     = 1;

    sendMessage();
}

void testWidget::on_pushButton_14_clicked()
{

    control     = 0;

    sendMessage();
}

void testWidget::on_pushButton_10_clicked()
{

    control     = 1;

    sendMessage();
}

void testWidget::on_pushButton_11_clicked()
{


    control     = 2;

    sendMessage();
}

void testWidget::on_pushButton_12_clicked()
{


    control = 3;

    sendMessage();
}

void testWidget::on_pushButton_13_clicked()
{


    control     = 4;

    sendMessage();
}

void testWidget::on_pushButton_15_clicked()
{
    static bool state = false;
    state ^=1;


    stateSVCH = state;

    sendMessage();
}

void testWidget::sendMessage()
{
    //exchangeBR->sendSettingsRLU(litera, modulation, reciever, control, stateSVCH, channelDistance);
}

void testWidget::on_pushButton_16_clicked()
{
    static QRegExp regex("^(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    if(regex.exactMatch(ui->lineEdit->text()))
    {
        channelDistance = ui->lineEdit->text().toUInt();
        sendMessage();
    }
    else
    {
        ui->lineEdit->setText(QLatin1String(""));
    }
}

void testWidget::on_radioButton_clicked(bool checked)
{
    Q_EMIT setSaveTofile(checked);
}


//void testWidget::updParam(SendSettings param)
//{
//    setSettingsRLU    (param.settingsRLU);
//    setModulRLU       (param.modulRLU);
//    setRecRLU         (param.recRLU);
//    setSignalRLU      (param.signalRLU);
//    setStateSVCHRLU   (param.stateSVCHRLU);
//    setChanDist       (param.chanDist);
//}

//Устанавливаем литеру
void testWidget::setSettingsRLU(int8_t state)
{
    if((state > 4) || (state < 1)) return;

    litera = state;

    ui->pushButton_2->setConstGreenPalette(false);
    ui->pushButton_3->setConstGreenPalette(false);
    ui->pushButton_4->setConstGreenPalette(false);
    ui->pushButton_5->setConstGreenPalette(false);
    switch (state)
    {
    case 0:{break;}
    case 1:{ui->pushButton_2->setConstGreenPalette(true);break;}
    case 2:{ui->pushButton_3->setConstGreenPalette(true);break;}
    case 3:{ui->pushButton_4->setConstGreenPalette(true);break;}
    case 4:{ui->pushButton_5->setConstGreenPalette(true);break;}
    default: break;
    }

}

void testWidget::setModulRLU(int8_t state)
{
    if((state > 1) || (state < 0)) return;

    modulation = state;

    ui->pushButton_6->setConstGreenPalette(false);
    ui->pushButton_7->setConstGreenPalette(false);

    switch (state)
    {
    case 0:{ui->pushButton_6->setConstGreenPalette(true);break;}
    case 1:{ui->pushButton_7->setConstGreenPalette(true);break;}

    default: break;
    }

}

void testWidget::setRecRLU(int8_t state)
{
    if((state > 1) || (state < 0)) return;

    reciever     = state;

    ui->pushButton_8->setConstGreenPalette(false);
    ui->pushButton_9->setConstGreenPalette(false);

    switch (state)
    {
    case 0:{ui->pushButton_8->setConstGreenPalette(true);break;}
    case 1:{ui->pushButton_9->setConstGreenPalette(true);break;}

    default: break;
    }
}

void testWidget::setSignalRLU(int8_t state)
{
    if((state > 4) || (state < 0)) return;
    control     = state;

    ui->pushButton_10->setConstGreenPalette(false);
    ui->pushButton_11->setConstGreenPalette(false);
    ui->pushButton_12->setConstGreenPalette(false);
    ui->pushButton_13->setConstGreenPalette(false);
    ui->pushButton_14->setConstGreenPalette(false);

    switch (state)
    {
    case 0:{ui->pushButton_14->setConstGreenPalette(true);break;}
    case 1:{ui->pushButton_10->setConstGreenPalette(true);break;}
    case 2:{ui->pushButton_11->setConstGreenPalette(true);break;}
    case 3:{ui->pushButton_12->setConstGreenPalette(true);break;}
    case 4:{ui->pushButton_13->setConstGreenPalette(true);break;}

    default: break;
    }
}

void testWidget::setStateSVCHRLU(int8_t state)
{
    if((state > 1) || (state < 0)) return;

    ui->pushButton_15->setConstGreenPalette(state);

    stateSVCH = state;
}

void testWidget::setChanDist(int16_t state)
{
    if((state > 256) || (state < 0)) return;

    channelDistance = state;
    ui->labelChannel->setText(QString::number(state));
}

void testWidget::on_pushButton_17_clicked()
{
    QDesktopServices::openUrl(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}
