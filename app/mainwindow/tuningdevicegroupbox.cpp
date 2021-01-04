#include "tuningdevicegroupbox.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QCheckBox>
#include "../core/messages/messagefinalkey.h"
#include "../core/messages/messagekeyselectionchanged.h"

TuningDeviceGroupBox::TuningDeviceGroupBox(Core *core, QWidget *parent) :
    DisplaySizeDependingGroupBox(parent, new QHBoxLayout, toFlag(MODE_TUNING)),
    MessageListener(),
    TuningDeviceController(core)
{
    setTitle(tr("Tuning device"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *mainLayout = qobject_cast<QHBoxLayout*>(mMainWidgetContainer->layout());
    mainLayout->setSpacing(0);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    mainLayout->addLayout(vboxLayout);

    QHBoxLayout *settingsLayout = new QHBoxLayout;
    vboxLayout->addLayout(settingsLayout);

    QHBoxLayout *suggestedValueLayout = new QHBoxLayout;
    settingsLayout->addLayout(suggestedValueLayout);
    suggestedValueLayout->setMargin(5);

    QLabel *suggestedValueLabel = new QLabel(tr("Impact:"));
    suggestedValueLayout->addWidget(suggestedValueLabel);
    suggestedValueLabel->setMargin(2);

    // Tuning device set value
    mSuggestedValue = new QSpinBox;
    mSuggestedValue->setRange(1, 100);
    mSuggestedValue->setSingleStep(1);
    mSuggestedValue->setSuffix("%");
    mSuggestedValue->setValue(1);
    //mSuggestedValue->setSizePolicy(QSizePolicy::Minimum);
    suggestedValueLayout->addWidget(mSuggestedValue);

    QHBoxLayout *autoTuningLayout = new QHBoxLayout;
    settingsLayout->addLayout(autoTuningLayout);
    autoTuningLayout->setMargin(5);
    QLabel * autoTuningLabel = new QLabel(tr("Auto:"));
    autoTuningLabel->setMargin(2);
    autoTuningLayout->addWidget(autoTuningLabel);

    mAutomaticTuning = new QCheckBox;
    autoTuningLayout->addWidget(mAutomaticTuning);
    QObject::connect(mAutomaticTuning, SIGNAL(toggled(bool)), this, SLOT(onAutoTuningToggled(bool)));

    mRunTuningCycle = new QPushButton;
    vboxLayout->addWidget(mRunTuningCycle);
    mRunTuningCycle->setText(tr("Run"));
    mRunTuningCycle->setDisabled(true);
    QObject::connect(mRunTuningCycle, SIGNAL(clicked(bool)), this, SLOT(onRunClicked()));


    QHBoxLayout *stateLayout = new QHBoxLayout;
    vboxLayout->addLayout(stateLayout);

    QLabel *stateLabel = new QLabel(tr("State:"));
    stateLayout->addWidget(stateLabel);

    mState = new QLabel("Idle");
    stateLayout->addWidget(mState);
    mState->setAlignment(Qt::AlignLeft);

}

void TuningDeviceGroupBox::onAutoTuningToggled(bool b) {
    mSuggestedValue->setDisabled(b);
}

void TuningDeviceGroupBox::onRunClicked() {
    mRunning = !mRunning;


    if (mRunning) {
        startAutoTuning();

        mRunTuningCycle->setText(tr("Stop"));
        mRunTuningCycle->setStyleSheet("background-color: crimson");
    } else {
        stopAutoTuning();

        mRunTuningCycle->setText(tr("Run"));
        mRunTuningCycle->setStyleSheet("");
    }
}

void TuningDeviceGroupBox::updateState(TuningDeviceController::STATES state) {
    switch (state) {
        case TuningDeviceController::DISABLED: mState->setText(tr("Disabled")); break;
        case TuningDeviceController::AWAIT_KEYPRESS: mState->setText(tr("Await keypress")); break;
        case TuningDeviceController::PERFORM_IMPACT: mState->setText(tr("Performing tuning")); break;
    }
}
//-----------------------------------------------------------------------------
//                             Message handler
//-----------------------------------------------------------------------------

void TuningDeviceGroupBox::handleMessage(MessagePtr m) {
    switch (m->getType())
    {
    case Message::MSG_FINAL_KEY: // Key has been recorded
    {
        auto message(std::static_pointer_cast<MessageFinalKey>(m));
        auto keyptr = message->getFinalKey();
        mKey = keyptr.get();

        keyRecorded();

        break;
    }
    /*case Message::MSG_KEY_SELECTION_CHANGED: {
        auto message(std::static_pointer_cast<MessageKeySelectionChanged>(m));
        mKey = message->getKey();

        if (mKey) {
            mRunTuningCycle->setDisabled(false);
        }
    }*/
    default:
        break;
    }
}

void TuningDeviceGroupBox::startAutoTuning() {
    start();
}

void TuningDeviceGroupBox::stopAutoTuning() {
    stop();
}
