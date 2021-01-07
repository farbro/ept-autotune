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
    LogI("Last impact direction: %d", lastImpactDirection);

    double target_hz = mTargetFrequency;
    double current_hz = mCurrentFrequency;
    double error_hz = current_hz - target_hz;
    LogI("Target: %f Hz, current: %f Hz", target_hz, current_hz);
    LogI("Error in Hz: %f", error_hz);

    double current_rad = freq2angle(current_hz, lastImpactDirection);
    double target_rad = freq2angle(target_hz, lastImpactDirection);
    double error_rad = current_rad - target_rad;
    LogI("Error in radians: %f", error_rad);

    double error_cents = deviationInCents(target_hz, current_hz);
    LogI("Error in cents: %f", error_cents);

    // Check if already in target
    if (fabs(error_cents) < maxDeviationCents) {
        emit updateState(TuningDeviceController::IN_TARGET);
        LogI("Already in target. Tuning finished.");
        return;
    }

    double minImpact;
    if (error_rad < 0) {
        minImpact = calculateRequiredImpactVelocity(current_rad, current_rad + 0.001);
    } else {
        minImpact = calculateRequiredImpactVelocity(current_rad, current_rad - 0.001);
    }

    LogI("Min velocity: %f", minImpact);

    double requiredImpactVelocity = calculateRequiredImpactVelocity(current_rad, target_rad);
    LogI("Required velocity: %f", requiredImpactVelocity);

    // Check if too close
    if (fabs(requiredImpactVelocity) <= fabs(minImpact)) {
        target_rad = current_rad + 0.001;

        LogI("Target is too close. New target is %f", target_rad);
    }

    requiredImpactVelocity = calculateRequiredImpactVelocity(current_rad, target_rad);
    LogI("Required velocity from angle %f to %f: %f", current_rad, target_rad, requiredImpactVelocity);

    sendEngageCommand(requiredImpactVelocity);
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

    // for string G4.1
    double angle_diff = targetAngle - currentAngle;

    if (angle_diff > 0.08) angle_diff = 0.08;
    if (angle_diff < -0.14)  angle_diff = -0.14;

    double vel = 0;

    if (angle_diff > 0) {
        vel = -4555.47*pow(angle_diff, 2) + 855.10*angle_diff + 20.598;

        if (vel > 60) vel = 60;
        if (vel < 45) vel = 45;
    } else if (angle_diff < 0) {
        vel = 755.53*pow(angle_diff, 2) + 380.34*angle_diff - 17.03;

        if (vel > -25) vel = -25;
        if (vel < -55) vel = -55;
    }

    return vel;
}


void TuningDeviceController::impactFinished() {

    if (curl->result() != CURLE_OK) {
        LogE("HTTP POST failed: %s", curl_easy_strerror(curl->result()));
    } else {
        LogI("HTTP POST succeeded.");
        mLastImpactVelocity = mCurrentImpactVelocity;

        mImpactInProggress = false;
        emit updateState(TuningDeviceController::AWAIT_KEYPRESS);
    }

}

void TuningDeviceController::tune() {
    performImpact();
}

void TuningDeviceController::setTargetFreq(double target) {
    LogI("Setting target frequency to %f Hz", target);
    mTargetFrequency = target;
}

void TuningDeviceController::setCurrentFreq(double current) {
    LogI("Setting current frequency to %f Hz", current);
    mCurrentFrequency = current;
}

double TuningDeviceController::deviationInCents(double target, double current) {
    double cents = 1200*log(current/target);

    return cents;
}
