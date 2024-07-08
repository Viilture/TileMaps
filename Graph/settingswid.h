#ifndef SETTINGSWID_H
#define SETTINGSWID_H

#pragma once

#include <QWidget>
#include <QString>
#include <QHostAddress>
#include <QListView>
#include <QDoubleSpinBox>
#include <QProcess>
#include <QStyledItemDelegate>

#include "globalpalette.h"
#include "rlicontrol.h"
#include "videocontrol.h"
#include "scenecontrol.h"
#include "vidcameradelegate.h"
#include "radardelegate.h"

namespace Ui {
class Settings;
}

class RLIControl;
class videoControl;

class SettingsWid : public QWidget
{
    Ui::Settings *ui;

    Q_OBJECT

    /** @brief num RLI */
    int32_t numRLI = 1;
    /** @brief num Camera */
    int32_t numCamera = 1;

    /** @brief класс управления радарами */
    RLIControl* rlucontrol = nullptr;

    /** @brief класс управления видео */
    videoControl* vidControl = nullptr;

    /** @brief Класс управления сценой */
    sceneControl& scenControl;

public:
    explicit SettingsWid(RLIControl* rlicontrol, videoControl* newVidContr, sceneControl& newScenContr, QWidget *parent = nullptr);
    virtual ~SettingsWid();

private Q_SLOTS:
    void on_lineEdit_5_textChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void onRemoveRLI3Clicked();

    void onPushButton4Clicked();

    void onPushButtonClicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_addCamera_clicked();

    void on_removeCamera_clicked();

    void on_lineIp4_3_textChanged(const QString &arg1);

    void on_lineLatCamera_valueChanged(double arg1);

    void on_lineAzimCamera_valueChanged(double arg1);

    void on_lineKrenCamera_valueChanged(double arg1);

    void on_lineTangCamera_valueChanged(double arg1);

    void on_addRLI_clicked();

    void on_removeRLI_clicked();

    void on_lineIp4_textChanged(const QString &arg1);

    void on_lineLat_valueChanged(double arg1);

    void on_lineLon_valueChanged(double arg1);

    void on_lineAzim_valueChanged(double arg1);

    void on_lineKren_valueChanged(double arg1);

    void on_lineTang_valueChanged(double arg1);

    void on_lineLonCamera_valueChanged(double arg1);

    void on_pushButtonSettings_clicked();

    void on_pushButtonRLU_clicked();

    void on_pushButtonCamera_clicked();

    void on_pushButtonMap_clicked();

    void on_pushButtonZone_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_listView_activated(const QModelIndex &index);

    void on_listViewRLU_clicked(const QModelIndex &index);

    void on_listViewRLU_activated(const QModelIndex &index);

    void on_listView_pressed(const QModelIndex &index);

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_listView_entered(const QModelIndex &index);

    void on_listViewRLU_doubleClicked(const QModelIndex &index);

    void on_listViewRLU_entered(const QModelIndex &index);

    void on_listViewRLU_pressed(const QModelIndex &index);

    void on_comboBox_activated(int index);

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

private:

    QString pass;
    /** @brief РЈСЃС‚Р°РЅРѕРІРєР° РїР°СЂРѕР»СЏ */
    void setPass(const QString &newPass);
    /** @brief Р‘Р»РѕРєРёСЂСѓРµРј РёРЅС‚РµСЂС„РµР№СЃ */
    void setBlocked(bool state);

    /** @brief РћР±РЅРѕРІР»СЏРµРј РїР°СЂР°РјРµС‚СЂС‹ */
    void updateParam();

protected:
    virtual void hideEvent(QHideEvent *event) override;

Q_SIGNALS:
    /** @brief РЈСЃС‚Р°РЅР°РІР»РёР°РµРј С‚РёРї РєР°СЂС‚С‹ */
    void setMapType(typeMaps newType);

    /**  @brief РЎРѕР·РґР°РµРј РЅРѕРІС‹Р№ Р Р°РґР°СЂ */
    void createNewRLI();

    /**  @brief РЎРѕР·РґР°РµРј РЅРѕРІСѓСЋ РєР°РјРµСЂСѓ */
    void createNewCamera();

public Q_SLOTS:

    /** @brief РЈСЃС‚Р°РЅРѕРѕРІРєР° СЃРѕСЃС‚РѕСЏРЅРёСЏ СЂР°Р±РѕС‚С‹ РІРёРґР¶РµС‚Р° */
    void setShow(bool state);
};

#endif // SETTINGS_H
