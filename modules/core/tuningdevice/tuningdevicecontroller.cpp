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


#include "tuningdevicecontroller.h"
#include "../core.h"
#include "../core/messages/messagekeyselectionchanged.h"
#include "../core/messages/messagefinalkey.h"
#include "../math/mathtools.h"
#include <sstream>
#include <QStringBuilder>

TuningDeviceController::TuningDeviceController(Core *core) : mCore(core) {
    curl = new CurlEasy();

    QObject::connect(curl, &CurlEasy::done, this, &TuningDeviceController::impactFinished);
}

TuningDeviceController::~TuningDeviceController() {}

void TuningDeviceController::start() {
    emit updateState(AWAIT_KEYPRESS);
    mEnabled = true;
    LogI("Tuning enabled");
}

void TuningDeviceController::stop() {
    emit updateState(DISABLED);
    mEnabled = false;
    LogI("Tuning disabled");
}

void TuningDeviceController::performImpact() {
    LogI("Begin tuning key");

    emit updateState(TuningDeviceController::PERFORM_IMPACT);

    int lastImpactDirection = 0;
    if (mLastImpactVelocity > 0) lastImpactDirection = 1;
    else if (mLastImpactVelocity < 0) lastImpactDirection = -1;
    double currentAngle = freq2angle(mKey->getTunedFrequency(), lastImpactDirection);
    double targetAngle = freq2angle(mKey->getComputedFrequency(), lastImpactDirection);

    double error = deviationInCents();
    LogI("Deviation = %f cents", error);

    // Check if already in target
    if (error < minErrorCents) {
        LogI("Already in target. Tuning finished.");
        return;
    }

    // Check if too close
    if (calculateRequiredImpactVelocity(currentAngle, targetAngle) < getMinImpact()) {
        targetAngle = currentAngle + getMinImpact();
    }

    double velocity = calculateRequiredImpactVelocity(currentAngle, targetAngle);

    sendEngageCommand(velocity);
}

bool TuningDeviceController::sendEngageCommand(double velocity) {
    mImpactInProggress = true;

    LogI("Engaging device with velocity %f", velocity);

    // Engange device through HTTP POST

    curl->set(CURLOPT_URL, DEVICE_ENGAGE_URL);
    curl->set(CURLOPT_TIMEOUT, 5L);

    // Set POST fields
    char params_arr[64];
    sprintf(params_arr, "motor_speed=%f", velocity);

    curl->set(CURLOPT_COPYPOSTFIELDS, params_arr);

    LogI("Sending HTTP POST");
    curl->perform();
    mCurrentImpactVelocity = velocity;

    return true;
}

double TuningDeviceController::freq2angle(double frequency, int lastTuningDirection = 0) {
    // TODO implement freq2angle mapping

    // FOR STRING G4.1
    double angle_cw = (frequency - 334.90)/262.255;
    double angle_ccw = (frequency - 337.85)/271.91;

    double angle;
    if (lastTuningDirection > 0) {
        angle = angle_cw;
    } else if (lastTuningDirection < 0) {
        angle = angle_ccw;
    } else {
        angle = (angle_cw + angle_ccw)/2;
    }

    LogI("frequency %f maps to angle %f", frequency, angle);

    return angle;
}

double TuningDeviceController::calculateRequiredImpactVelocity(double currentAngle, double targetAngle) {
    // TODO implement mapping
    double vel;

    vel = 20;

    LogI("Required velocity from angle %f to %f: %f", currentAngle, targetAngle, vel);
    return vel;
}

double TuningDeviceController::getMinImpact() {
    return 0;
}


void TuningDeviceController::impactFinished() {

    if (curl->result() != CURLE_OK) {
        LogE("HTTP POST failed: %s", curl_easy_strerror(curl->result()));
    } else {
        LogI("HTTP POST succeeded.");
        mLastImpactVelocity = mCurrentImpactVelocity;
    }

    mImpactInProggress = false;
    emit updateState(TuningDeviceController::AWAIT_KEYPRESS);
}

void TuningDeviceController::tune() {
    performImpact();
}

void TuningDeviceController::setKey(const Key *key) {
    mKey = key;
}

double TuningDeviceController::deviationInCents() {
    double freq1 = mKey->getComputedFrequency();
    double freq2 = mKey->getTunedFrequency();

    double cents = 1200*log(freq2/freq1);

    return cents;
}
