#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = managerSignals::create();

    init();

    widTest = QSharedPointer<testWidget>::create();
    widTest->show();
    widTest->raise();

    /** @brief Управление сигналами */
    manager->allConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    //Установка палетты
    white_Palette = qApp->palette();
    globalPalette gPalette;
    dark_Palette = gPalette.getDarkPalette();
    qApp->setPalette(dark_Palette);

    // Класс управления сценой
    scenContr = new sceneControl(this);

    /** @brief Запись видео рабочего стола*/
    videoLog = VideoLogging::create();

    // Класс управления видео
    videoContr = QSharedPointer<videoControl>::create(this, scenContr);
    if(videoContr->getCameraWid() != nullptr)
        videoContr->getCameraWid()->hide();

    // Управление радаром
    rliContr = QSharedPointer<RLIControl>::create(this, scenContr);

    /** @brief Третичная обработка */
    TertBuffer = QSharedPointer<TertiaryProcessingBuffer>::create();

    auto shMapContr = scenContr->getMapContr();
    if(!shMapContr.isNull())
    TertBuffer->setMapContr(scenContr->getMapContr());


    /** @brief Виджет настроек РЛУ */
    //Виджет настроек РЛУ
    widSetting = QSharedPointer<SettingsWid>::create(rliContr.data(), videoContr.data(), *scenContr);

    /** @brief Cсылка на класс управления сценой */
    widJournal = QSharedPointer<Journal>::create();
    widJournal->setTableModel(TertBuffer.toWeakRef());

    QSplitter *splitterScene = new QSplitter(Qt::Vertical);
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitterScene->setHandleWidth(0);
    splitter->setHandleWidth(0);
    splitterScene->addWidget(scenContr);

    splitter->addWidget(splitterScene);
    if(videoContr->getCameraWid() != nullptr)
        splitter->addWidget(videoContr->getCameraWid());

    ui->horizontalLayout->addWidget(splitter);

    //Настраиваем контекстное меню
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(scenContr, &sceneControl::setShowSettings, widSetting.data(), &SettingsWid::setShow);
    connect(widSetting.data(), &SettingsWid::setMapType,  scenContr->getMapContr().data(), &MapControl::setMapType);

    //Вкл/Откл отображения камеры
    connect(scenContr, &sceneControl::setStateCamera, this, [=](bool state)
    {
        if(!videoContr.isNull())
        {
            if(videoContr->getCameraWid() != nullptr)
            {
                if(state)
                {
                    videoContr->getCameraWid()->setGeometry(0,0 ,width()/2, height()/2);
                    videoContr->getCameraWid()->show();
                }
                else videoContr->getCameraWid()->hide();
            }
        }
    });
    connect(scenContr, &sceneControl::setShowJournal, this, [=](bool state)
    {
        if(!widJournal.isNull())
        {
            if(state)
            {
                widJournal->setShow(true);
            }
            else widJournal->setShow(false);
        }
    });
}

void MainWindow::keyPressEvent(QKeyEvent *evt)
{
    int key = ((int)evt->key() | (int)evt->modifiers());

    switch(key)
    {
    case (Qt::Key_Escape)://Выключение программы
    {

        if(QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("Закрытие АРМ РЛМ"), QStringLiteral("Уверены"), QMessageBox::Yes|QMessageBox::No))
        {
            qApp->quit();
        }
        break;
    }
    case ((Qt::Key_X|Qt::ControlModifier))://Выключение программы
    {

        if(QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("Закрытие АРМ РЛМ"), QStringLiteral("Уверены"), QMessageBox::Yes|QMessageBox::No))
        {
            qApp->quit();
        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
}
