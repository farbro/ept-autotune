/*****************************************************************************
 * Copyright 2020 Mats Jonsson
 *
 * This file is part of Entropy Piano Tuner.
 *
 * Entropy Piano Tuner is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Entropy Piano Tuner is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Entropy Piano Tuner. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef TUNINGDEVICECONTROLLER_H
#define TUNINGDEVICECONTROLLER_H

#include "../piano/key.h"
#include "../system/simplethreadhandler.h"
#include "../../core/messages/messagelistener.h"
#include "../../../thirdparty/qtcurl/src/CurlEasy.h"

#define DEVICE_ENGAGE_URL "http://pianotuner.mnm.granet/run"

class Core;
class CurlEasy;

class TuningDeviceController : public QObject
{
    Q_OBJECT
public:

    enum STATES {
        DISABLED,
        AWAIT_KEYPRESS,
        PERFORM_IMPACT,
        IN_TARGET
    };

    TuningDeviceController(Core *core);
    virtual ~TuningDeviceController();

    void start();
    void stop();
    void setTargetFreq(double target);
    void setCurrentFreq(double current);

    void tune();

signals:
    void updateState(STATES state);


private:
    Core *mCore;
    CurlEasy *curl = nullptr;
    QString mParams;
    double mCurrentImpactVelocity = 0;
    double mLastImpactVelocity = 0;

    double mTargetFrequency;
    double mCurrentFrequency;

    bool mEnabled = false;
    bool mImpactInProggress = false;
    STATES mState = DISABLED;
    double maxDeviationCents = 3;
    double calculateError();
    double freq2angle(double frequency, int lastTuningDirection);
    void performImpact();
    double calculateRequiredImpactVelocity(double currentAngle, double targetAngle);
    double getMinImpact();
    bool sendEngageCommand(double velocity);
    double deviationInCents(double target, double current);

private slots:
    void impactFinished();

};

#endif // TUNINGDEVICECONTROLLER_H
