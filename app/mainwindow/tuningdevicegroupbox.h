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
                             public MessageListener,
                             public TuningDeviceController
{
    Q_OBJECT

public:
    TuningDeviceGroupBox(Core *core, QWidget *parent);

    void setSuggestedValue(double value) {mSuggestedValue->setValue(value);}
    void updateState(TuningDeviceController::STATES state) override;

private:
    QSpinBox *mSuggestedValue;
    QPushButton *mRunTuningCycle;
    QCheckBox *mAutomaticTuning;
    QLabel *mState;
    bool mRunning = false;
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
};

#endif // TUNINGDEVICEGROUPBOX_H
