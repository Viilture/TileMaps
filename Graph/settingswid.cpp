#include "settingswid.h"
#include "ui_settings.h"
#include "managersignals.h"

SettingsWid::SettingsWid(RLIControl* rlicontrol, videoControl* newVidContr, sceneControl& newScenContr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings),
    rlucontrol(rlicontrol),
    vidControl(newVidContr),
    scenControl(newScenContr)
{
    ui->setupUi(this);

    //Устанавливаем вход только по паролю
    setBlocked(true);

    //Устанавливаем пароль
    setPass(QStringLiteral("Barier"));

    globalPalette glPalette;
    setPalette(glPalette.getStandartDarkPalette());

    hide();

    //Устанавливаем в ComboBox модели всех радаров и камер

    vidCameraDelegate* delVid = new vidCameraDelegate();
    if(delVid != nullptr)
        ui->listView->setItemDelegate(delVid);

    RadarDelegate* delRLU = new RadarDelegate();
    if(delRLU != nullptr)
        ui->listViewRLU->setItemDelegate(delRLU);

    if(vidControl != nullptr)
    {
        ui->listView->setModel(vidControl);
    }
    if(rlicontrol != nullptr)
    {
        ui->listViewRLU->setModel(rlicontrol);
    }


    //Видимость поверх других окон
    setWindowFlags(this->windowFlags()|Qt::WindowStaysOnTopHint);

    QSharedPointer<managerSignals> manager = managerSignals::create();
    manager->addWidSettings(this);

    ui->pushButtonCamera->click();

    static QTimer t1;

    connect(&t1, &QTimer::timeout, this, [=]()
    {
        static int a =-32;
        a++;
        if(a>32) a = -32;

        if(a<0)
        {
            ui->addRLI->click();
            ui->addCamera->click();
        }
        else
        {
            ui->removeRLI->click();
            ui->removeCamera->click();
        }

    });

    // t1.start(20);

}

SettingsWid::~SettingsWid()
{
    delete ui;
}

void SettingsWid::on_lineEdit_5_textChanged(const QString &arg1)
{
    if(arg1.isEmpty() || arg1.isNull()) return;

    //Если пароль правильный, то разблокируем интерфейс
    if(pass == arg1)
    {
        setBlocked(false);
        ui->lineEdit_5->clear();
    }
}

/** @brief Устаноовка состояния работы виджета */
void SettingsWid::setShow(bool state)
{
    if(state)
    {
        show();
        raise();
    }
    else
        hide();
}

/** @brief Установка пароля */
void SettingsWid::setPass(const QString &newPass)
{
    pass = newPass;
}

/** @brief Блокируем интерфейс */
void SettingsWid::setBlocked(bool state)
{
    if(state)
    {
        ui->widget->show();
        ui->widget_2->hide();
    }
    else
    {
        ui->widget->hide();
        ui->widget_2->show();
    }
}

/** @brief Обновляем параметры */
void SettingsWid::updateParam()
{
    //Обновляем параметры Радара
    if(rlucontrol != nullptr)
    {
        if((numRLI >= 1) && (numRLI <= rlucontrol->getRadarSize()))
        {
            QString qsRluName = rlucontrol->getName(numRLI);
            if((!qsRluName.isEmpty()) && (!qsRluName.isNull()))
                ui->RluName->setText(qsRluName);

            QString qsIp4 = rlucontrol->getIp4(numRLI);
            if((!qsIp4.isEmpty()) && (!qsIp4.isNull()))
                ui->lineIp4->setText(qsIp4);

            ui->lineAzim->setValue(rlucontrol->getAzim(numRLI));
            ui->lineKren->setValue(rlucontrol->getKren(numRLI));
            ui->lineTang->setValue(rlucontrol->getTang(numRLI));
            ui->lineLat->setValue(rlucontrol->getLat(numRLI));
            ui->lineLon->setValue(rlucontrol->getLong(numRLI));

            int32_t liter = rlucontrol->getLitera(numRLI);

            switch (liter)
            {
            case 1:
            {

                break;
            }
            case 2:
            {

                break;
            }
            case 3:
            {

                break;
            }
            case 4:
            {

                break;
            }
            default:
            {
                break;
            }
            }
        }
        else
        {
            ui->RluName->setText(QLatin1String(""));
            ui->lineAzim->setValue(0.0);
            ui->lineKren->setValue(0.0);
            ui->lineTang->setValue(0.0);
            ui->lineLat->setValue(0.0);
            ui->lineLon->setValue(0.0);
            ui->lineIp4->setText(QStringLiteral("192.168.0.220"));
        }
    }

    if(vidControl != nullptr)
    {
        //Обновляем параметры камеры
        if((numCamera >= 1) && (numCamera <= vidControl->getCameraSize()))
        {

            ui->labelCamera->setText(vidControl->getName(numCamera));
            ui->lineAzimCamera->setValue(vidControl->getAzim(numCamera));
            ui->lineKrenCamera->setValue(vidControl->getKren(numCamera));
            ui->lineTangCamera->setValue(vidControl->getTang(numCamera));
            ui->lineLatCamera->setValue(vidControl->getLat(numCamera));
            ui->lineLonCamera->setValue(vidControl->getLong(numCamera));
            ui->lineIp4_3->setText(vidControl->getIp4(numCamera).toString());
        }
        else
        {
            ui->labelCamera->setText(QLatin1String(""));
            ui->lineAzimCamera->setValue(0.0);
            ui->lineKrenCamera->setValue(0.0);
            ui->lineTangCamera->setValue(0.0);
            ui->lineLatCamera->setValue(0.0);
            ui->lineLonCamera->setValue(0.0);
            ui->lineIp4_3->setText(QStringLiteral("192.168.2.197"));
        }
    }
}

void SettingsWid::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    setBlocked(true); //Устанавливаем пароль
}

void SettingsWid::on_pushButton_5_clicked()
{

}

void SettingsWid::on_addRLI_clicked()
{
    Q_EMIT createNewRLI();
}

void SettingsWid::on_removeRLI_clicked()
{
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    rlucontrol->removeRadar(numRLI);

    int32_t num = numRLI++;
    if((num >= 0) && (num <= rlucontrol->getRadarSize()))
    {
        numRLI = num;
    }
    else
    {
        numRLI = 1;
    }

    updateParam();
    rlucontrol->selectRadar(numRLI);
}

void SettingsWid::on_lineAzim_valueChanged(double azim)
{
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    if(azim < 0 || azim > 360) return;

    rlucontrol->setAzim(numRLI, azim);
}

void SettingsWid::on_lineKren_valueChanged(double kren)
{
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    if(kren < 0 || kren > 360) return;

    rlucontrol->setKren(numRLI, kren);
}

void SettingsWid::on_lineTang_valueChanged(double tang)
{
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    if(tang < 0 || tang > 360) return;

    rlucontrol->setTang(numRLI, tang);
}

void SettingsWid::on_lineLat_valueChanged(double lat)
{
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    if(lat < -180 || lat > 180) return;

    rlucontrol->setLat(numRLI, lat);
}


void SettingsWid::on_lineLon_valueChanged(double lon)
{
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    if(lon < -180 || lon > 180) return;

    rlucontrol->setLong(numRLI, lon);
}

void SettingsWid::on_lineIp4_textChanged(const QString &arg1)
{
    static QString oldIp4(QStringLiteral("192.168.0.1"));

    int cursorPosition = ui->lineIp4->cursorPosition();

    //Проверяем соответсвтие строки Ip4
    static QRegExp regex("^((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]?)$");
    if(regex.exactMatch(arg1))
    {

        QHostAddress newHostAddr = QHostAddress(arg1);
        if((!newHostAddr.isGlobal()) && (!newHostAddr.isLinkLocal())) return;

        oldIp4 = arg1;
        ui->lineIp4->setText(oldIp4);
        rlucontrol->setIp4(numRLI, oldIp4);
    }
    else
    {
        ui->lineIp4->setText(oldIp4);
    }
    ui->lineIp4->setCursorPosition(cursorPosition);
}

void SettingsWid::onRemoveRLI3Clicked()
{
    //Запускаем командную строку с бесконечным пингованием сервера

    QSharedPointer<QProcess> process = QSharedPointer<QProcess>::create();
    //Запускаем cmd
    process->startDetached(QStringLiteral("cmd.exe"), QStringList() << QStringLiteral("/C") << QStringLiteral("ping -t 192.168.0.20"));

}

void SettingsWid::onPushButton4Clicked()
{
    Q_EMIT setMapType(typeMaps::osmMaps);
}

void SettingsWid::onPushButtonClicked()
{
    Q_EMIT setMapType(typeMaps::googleGEOMaps);
}

void SettingsWid::on_pushButton_2_clicked()
{
    Q_EMIT setMapType(typeMaps::googleMaps);
}

void SettingsWid::on_pushButton_3_clicked()
{
    Q_EMIT setMapType(typeMaps::statmen);
}

void SettingsWid::on_addCamera_clicked()
{

    Q_EMIT createNewCamera();
}

void SettingsWid::on_removeCamera_clicked()
{
    if((vidControl == nullptr) || (numCamera < 1)) return;

    vidControl->removeCamera(numCamera);

    int32_t num = numCamera++;
    if((num >= 0) && (num <= vidControl->getCameraSize()))
    {
        numCamera = num;
    }
    else
    {
        numCamera = 1;
    }

    updateParam();
    vidControl->selectCamera(numCamera);

}

void SettingsWid::on_lineLatCamera_valueChanged(double lat)
{
    if((vidControl == nullptr) || (numCamera < 1)) return;

    if(lat < -180 || lat > 180) return;

    vidControl->setLat(numCamera, lat);
}

void SettingsWid::on_lineAzimCamera_valueChanged(double azim)
{
    if((vidControl == nullptr) || (numCamera < 1)) return;

    if(azim < 0 || azim > 360) return;

    vidControl->setAzim(numCamera, azim);
}

void SettingsWid::on_lineKrenCamera_valueChanged(double kren)
{
    if((vidControl == nullptr) || (numCamera < 1)) return;

    if(kren < 0 || kren > 360) return;

    vidControl->setKren(numCamera, kren);
}

void SettingsWid::on_lineTangCamera_valueChanged(double tang)
{
    if((vidControl == nullptr) || (numCamera < 1)) return;

    if(tang < 0 || tang > 360) return;

    vidControl->setTang(numCamera, tang);
}

void SettingsWid::on_lineIp4_3_textChanged(const QString &arg1)
{
    static QHostAddress oldIp4(QStringLiteral("192.168.2.197"));

    int cursorPosition = ui->lineIp4->cursorPosition();

    //Проверяем соответсвтие строки Ip4
    static QRegExp regex("^((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]?)$");
    if(regex.exactMatch(arg1))
    {

        QHostAddress adr;
        if((!adr.isGlobal()) && (!adr.isLinkLocal())) return;

        adr.setAddress(arg1);
        oldIp4.setAddress(arg1);
        ui->lineIp4_3->setText(adr.toString());
        vidControl->setIp4(numCamera, oldIp4);
    }
    else
    {
        ui->lineIp4_3->setText(oldIp4.toString());
    }
    ui->lineIp4_3->setCursorPosition(cursorPosition);
}

void SettingsWid::on_lineLonCamera_valueChanged(double lon)
{
    if((vidControl == nullptr) || (numCamera < 1)) return;

    if(lon < -180 || lon > 180) return;

    vidControl->setLong(numCamera, lon);
}


void SettingsWid::on_pushButtonSettings_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);

    ui->pushButtonSettings->setConstGreenPalette(true);
    ui->pushButtonRLU     ->setConstGreenPalette(false);
    ui->pushButtonCamera  ->setConstGreenPalette(false);
    ui->pushButtonMap     ->setConstGreenPalette(false);
    ui->pushButtonZone    ->setConstGreenPalette(false);
}


void SettingsWid::on_pushButtonRLU_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    ui->pushButtonSettings->setConstGreenPalette(false);
    ui->pushButtonRLU     ->setConstGreenPalette(true);
    ui->pushButtonCamera  ->setConstGreenPalette(false);
    ui->pushButtonMap     ->setConstGreenPalette(false);
    ui->pushButtonZone    ->setConstGreenPalette(false);
}


void SettingsWid::on_pushButtonCamera_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    ui->pushButtonSettings->setConstGreenPalette(false);
    ui->pushButtonRLU     ->setConstGreenPalette(false);
    ui->pushButtonCamera  ->setConstGreenPalette(true);
    ui->pushButtonMap     ->setConstGreenPalette(false);
    ui->pushButtonZone    ->setConstGreenPalette(false);
}


void SettingsWid::on_pushButtonMap_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);

    ui->pushButtonSettings->setConstGreenPalette(false);
    ui->pushButtonRLU     ->setConstGreenPalette(false);
    ui->pushButtonCamera  ->setConstGreenPalette(false);
    ui->pushButtonMap     ->setConstGreenPalette(true);
    ui->pushButtonZone    ->setConstGreenPalette(false);
}


void SettingsWid::on_pushButtonZone_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);

    ui->pushButtonSettings->setConstGreenPalette(false);
    ui->pushButtonRLU     ->setConstGreenPalette(false);
    ui->pushButtonCamera  ->setConstGreenPalette(false);
    ui->pushButtonMap     ->setConstGreenPalette(false);
    ui->pushButtonZone    ->setConstGreenPalette(true);
}

void SettingsWid::on_listView_clicked(const QModelIndex &index)
{
    numCamera = index.row() + 1;

    //Перемещаемся к радару на карте
    if((vidControl == nullptr) || (numCamera < 1)) return;


    updateParam();
    vidControl->selectCamera(numCamera);
}

void SettingsWid::on_listView_activated(const QModelIndex &index)
{
    numCamera = index.row() + 1;

    //Перемещаемся к радару на карте
    if((vidControl == nullptr) || (numCamera < 1)) return;


    updateParam();
    vidControl->selectCamera(numCamera);
}

void SettingsWid::on_listView_pressed(const QModelIndex &index)
{
    numCamera = index.row() + 1;

    //Перемещаемся к радару на карте
    if((vidControl == nullptr) || (numCamera < 1)) return;


    updateParam();
    vidControl->selectCamera(numCamera);
}


void SettingsWid::on_listView_doubleClicked(const QModelIndex &index)
{
    numCamera = index.row() + 1;

    //Перемещаемся к радару на карте
    if((vidControl == nullptr) || (numCamera < 1)) return;


    updateParam();
    vidControl->selectCamera(numCamera);
}


void SettingsWid::on_listView_entered(const QModelIndex &index)
{
    numCamera = index.row() + 1;

    //Перемещаемся к радару на карте
    if((vidControl == nullptr) || (numCamera < 1)) return;


    updateParam();
    vidControl->selectCamera(numCamera);
}

void SettingsWid::on_listViewRLU_clicked(const QModelIndex &index)
{
    numRLI = index.row() + 1;

    //Перемещаемся к радару на карте
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    updateParam();
    rlucontrol->selectRadar(numRLI);
}


void SettingsWid::on_listViewRLU_activated(const QModelIndex &index)
{

    numRLI = index.row() + 1;

    //Перемещаемся к радару на карте
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    updateParam();
    rlucontrol->selectRadar(numRLI);

}



void SettingsWid::on_listViewRLU_doubleClicked(const QModelIndex &index)
{
    numRLI = index.row() + 1;

    //Перемещаемся к радару на карте
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    updateParam();
    rlucontrol->selectRadar(numRLI);
}


void SettingsWid::on_listViewRLU_entered(const QModelIndex &index)
{
    numRLI = index.row() + 1;

    //Перемещаемся к радару на карте
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    updateParam();
    rlucontrol->selectRadar(numRLI);
}


void SettingsWid::on_listViewRLU_pressed(const QModelIndex &index)
{
    numRLI = index.row() + 1;

    //Перемещаемся к радару на карте
    if((rlucontrol == nullptr) || (numRLI < 1)) return;

    updateParam();
    rlucontrol->selectRadar(numRLI);
}


void SettingsWid::on_comboBox_activated(int index)
{
    switch (index) {
    case 0:
    {
        scenControl.setCordSystem(CoordinatSystem::WGS84Geodez);
        break;
    }
    case 1:
    {
        scenControl.setCordSystem(CoordinatSystem::WGS84GeoCentr);
        break;
    }
    case 2:
    {
        scenControl.setCordSystem(CoordinatSystem::WGS84GeoZone);
        break;
    }
    default:
        break;
    }
}


void SettingsWid::on_pushButton_4_clicked()
{
        Q_EMIT setMapType(typeMaps::osmMaps);
}


void SettingsWid::on_pushButton_clicked()
{
        Q_EMIT setMapType(typeMaps::googleGEOMaps);
}

