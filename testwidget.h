#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QRegExp>
#include <QThread>
#include <QDir>
#include <QDesktopServices>
#include <QSharedPointer>

#include "Exchange/Barier2Exchange.h"
#include "rlibuffer.h"
#include "settingsBR.h"

namespace Ui {
class testWidget;
}

class Barier2Exchange;
class RLIbuffer;

class testWidget : public QWidget
{
    Q_OBJECT
    QThread thread;

    QSharedPointer<RLIbuffer> buf;

public:
    explicit testWidget(QWidget *parent = nullptr);
    virtual ~testWidget();

private Q_SLOTS:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_radioButton_clicked(bool checked);

private:
    Ui::testWidget *ui;

    QSharedPointer<Barier2Exchange> exchangeBR;

    uint8_t litera           = 1;
    uint8_t modulation       = 1;
    uint8_t reciever         = 1;
    uint8_t control          = 0;
    uint8_t stateSVCH        = 0;
    uint8_t channelDistance  = 1;

    void sendMessage();

Q_SIGNALS:
    void setSaveTofile(bool newSaveTofile);

public Q_SLOTS:

   // void updParam(SendSettings param);

    void setSettingsRLU(int8_t state);
    void setModulRLU(int8_t state);
    void setRecRLU(int8_t state);
    void setSignalRLU(int8_t state);
    void setStateSVCHRLU(int8_t state);
    void setChanDist(int16_t state);
};

#endif // TESTWIDGET_H
