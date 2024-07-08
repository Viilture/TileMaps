#ifndef TARGETCHARACTERISTICS_H
#define TARGETCHARACTERISTICS_H

#include <QWidget>

namespace Ui {
class TargetCharacteristics;
}

class TargetCharacteristics : public QWidget
{
    Q_OBJECT

public:
    explicit TargetCharacteristics(QWidget *parent = nullptr);
    ~TargetCharacteristics();

private:
    Ui::TargetCharacteristics *ui;
};

#endif // TARGETCHARACTERISTICS_H
