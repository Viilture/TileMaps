#ifndef BUTTONSETTINGS_H
#define BUTTONSETTINGS_H

#include <QPushButton>
#include <QPainter>
#include <QWidget>

#include <globalpalette.h>

class ButtonSettings : public QPushButton
{
    Q_OBJECT

    /** @brief Возможность изменения QPalette */
    bool stateGreen = false;
    int stateClick = false;
    int stateEnter = false;


public:
    explicit ButtonSettings(QWidget* newParent);
    virtual ~ButtonSettings();

    /** @brief Возможность изменения QPalette */
    void setConstGreenPalette(bool newState);

    // QWidget interface
    bool getStateGreen() const;

protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // BUTTONSETTINGS_H
