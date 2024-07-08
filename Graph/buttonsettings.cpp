#include "buttonsettings.h"


ButtonSettings::ButtonSettings(QWidget* newParent):QPushButton(newParent)
{
    setIconSize(QSize(40, 40));
    setFlat(true); //Для прозрачности фона
    setAutoFillBackground(true);
    QFont fontInit(QStringLiteral("Arial"), 16, 4);
    setFont(fontInit);

}

ButtonSettings::~ButtonSettings()
{

}

/** @brief Возможность изменения QPalette */
void ButtonSettings::setConstGreenPalette(bool newState)
{
    stateGreen = std::move(newState);
    globalPalette palette;
    if(stateGreen) setPalette(palette.getGreenPalette());
    else           setPalette(palette.getDarkPalette());
}

bool ButtonSettings::getStateGreen() const
{
    return stateGreen;
}

void ButtonSettings::enterEvent(QEvent *event)
{
    QPushButton::enterEvent(event);
    globalPalette palette;
    if(stateGreen) setPalette(palette.getGreenPalette());
    else setPalette(palette.getDarkPalette2());
}

void ButtonSettings::leaveEvent(QEvent *event)
{
    QPushButton::leaveEvent(event);
    globalPalette palette;
    if(stateGreen) setPalette(palette.getGreenPalette());
    else setPalette(palette.getDarkPalette());

}

void ButtonSettings::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);
    globalPalette palette;
    setPalette(palette.getGreenPalette());

}

void ButtonSettings::mouseReleaseEvent(QMouseEvent *event)
{
    QPushButton::mouseReleaseEvent(event);
    globalPalette palette;
    if(stateGreen) setPalette(palette.getGreenPalette());
    else setPalette(palette.getDarkPalette());
}

void ButtonSettings::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.end();

    QPushButton::paintEvent(event);
}
