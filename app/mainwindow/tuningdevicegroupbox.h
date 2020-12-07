#ifndef TUNINGDEVICEGROUPBOX_H
#define TUNINGDEVICEGROUPBOX_H

#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include "prerequisites.h"
#include "widgets/displaysizedependinggroupbox.h"

class TuningDeviceGroupBox : public DisplaySizeDependingGroupBox
{
    Q_OBJECT
public:
    TuningDeviceGroupBox(QWidget *parent);

    void setSuggestedValue(double value) {mSuggestedValue->setValue(value);}

private:
    QSpinBox *mSuggestedValue;
    QPushButton *mRunTuningCycle;
    QCheckBox *mAutomaticTuning;
    QLabel *mState;

signals:
    void runClicked();
    void autoTuningToggled(bool);

public slots:
    void setState(int state);

private slots:
    void onRunClicked();
    void onAutoTuningToggled(bool);
};

#endif // TUNINGDEVICEGROUPBOX_H
