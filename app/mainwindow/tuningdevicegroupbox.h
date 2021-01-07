#ifndef TUNINGDEVICEGROUPBOX_H
#define TUNINGDEVICEGROUPBOX_H

#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QCheckBox>
#include "prerequisites.h"
#include "widgets/displaysizedependinggroupbox.h"
#include "core/messages/messagelistener.h"
#include "core.h"
#include "core/tuningdevice/tuningdevicecontroller.h"

class TuningDeviceGroupBox : public DisplaySizeDependingGroupBox,
                             public MessageListener
{
    Q_OBJECT

public:
    TuningDeviceGroupBox(Core *core, QWidget *parent);

    void setSuggestedValue(double value) {mSuggestedValue->setValue(value);}

private:
    TuningDeviceController mController;
    QSpinBox *mSuggestedValue;
    QPushButton *mRunTuningCycle;
    QCheckBox *mAutomaticTuning;
    QLabel *mState;
    const Key *mKey = nullptr;

    ///////////////////////////////////////////////////////////////////////////////
    /// \brief Message handling.
    /// \param m : The message.
    ///
    ///////////////////////////////////////////////////////////////////////////////
    virtual void handleMessage(MessagePtr m) override final;

    void startAutoTuning();
    void stopAutoTuning();

private slots:
    void onRunClicked();
    void onAutoTuningToggled(bool);
    void onStateChanged(TuningDeviceController::STATES state);
};

#endif // TUNINGDEVICEGROUPBOX_H
