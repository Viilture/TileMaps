#include "videowidget.h"
#include "ui_videowidget.h"

videoWidget::videoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::videoWidget)
{
    ui->setupUi(this);

    //    QWindow* wid = QWindow::fromWinId((WId)9766516);

    //    wid3 = QWidget::createWindowContainer(wid, nullptr, Qt::Widget);

    //    ui->gridLayout_3->addWidget(wid3);

    vidExch = QSharedPointer<VideoExchange>::create(nullptr, modeExchange::client, 3999, "127.0.0.1", 56, 789);
    //vidExch->moveToThread(&threadVideoExchange);

    connect(vidExch.data(), &VideoExchange::readyReadCmdState, this, &videoWidget::readyReadCmdState);

    //    QProcess process;
    //    process.setProgram("C:/QtProg/videoGrabberQML/build-VideoGrabQML-Desktop_Qt_5_12_12_MinGW_64_bit-Release/release/VideoGrabQML.exe");

    //    process.start();

    //    init();
}

videoWidget::~videoWidget()
{
    threadVideoExchange.quit();
    threadVideoExchange.wait();

    delete ui;
}

/** @brief Устанавливаем новый источник видео */
void videoWidget::setNewSourceVideo(QHostAddress ip)
{

}

void videoWidget::init()
{
    //Данный флаг необходим для захвата виджета камеры, после захвата активируется keyPressEvent
    setFocusPolicy(Qt::StrongFocus);

    QSettings set(QCoreApplication::applicationDirPath() + Barier2INI, QSettings::IniFormat);

    //Базоваый Ip4 адрес камеры
    ip4addr = set.value(QStringLiteral("Camera/ip4"), "192.168.2.197").toString();

    ip4addr.setAddress(QStringLiteral("192.168.2.197"));
}

/** @class Текущее состояние модуля камеры */
void videoWidget::readyReadCmdState(QSharedPointer<CmdState> send)
{
    if(send.isNull()) return;

    static bool a = true;

    if(a)
    {
        qDebug() << 1;
        static QWindow* wid;


        static QTimer t1;

        connect(&t1, &QTimer::timeout, this, [=]()
        {
            wid = QWindow::fromWinId((WId)send->winID);
            t1.stop();
        });


        t1.start(300);

        static QTimer t2;

        connect(&t2, &QTimer::timeout, this, [=]()
        {
            wid3 = QWidget::createWindowContainer(wid, nullptr);
            ui->verticalLayout->addWidget(wid3);
            t2.stop();
        });

        t2.start(1000);

        a = false;
    }
}
