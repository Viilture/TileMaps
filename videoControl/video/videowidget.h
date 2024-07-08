#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSharedPointer>
#include <QHostAddress>
#include <QThread>
#include <QWindow>
#include <QKeyEvent>
#include <QPainter>
#include <QProcess>

#include "buttonsettings.h"
#include "abstractcamera.h"
#include "pelcocamera.h"

#include "Exchange/videoexchange.h"

#define Barier2INI "/Barier2INI"

class VideoExchange;

namespace Ui
{
class videoWidget;
}

class videoWidget : public QWidget
{
    Q_OBJECT

    QWidget* wid3;

    QHostAddress ip4addr = QHostAddress(QStringLiteral("192.168.2.197"));

    QSharedPointer<VideoExchange> vidExch;

    QThread threadVideoExchange;


public:
    explicit videoWidget(QWidget *parent = nullptr);
    virtual ~videoWidget();

    /** @brief Устанавливаем новый источник видер */
    void setNewSourceVideo(QHostAddress ip);

private:
    Ui::videoWidget *ui;

    QStringList streamArgs;

    void init();

Q_SIGNALS:

    void readyReadCmdSetIP(QSharedPointer<CmdSetState> send);
    void readyReadCmdPowerOFF(QSharedPointer<CmdPowerOFF> send);
    void readyReadCmdRotateCamera(QSharedPointer<CmdRotateCamera> send);

public Q_SLOTS:

    /** @class Текущее состояние модуля камеры */
    void readyReadCmdState(QSharedPointer<CmdState> send);

};

#endif // VIDEOWIDGET_H
