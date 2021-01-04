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
#include <sstream>

TuningDeviceController::TuningDeviceController(Core *core) : mCore(core) {
}

TuningDeviceController::~TuningDeviceController() {}

void TuningDeviceController::start() {
    mEnabled = true;
    LogI("Tuning enabled");
}

void TuningDeviceController::stop() {
    mEnabled = false;
    LogI("Tuning disabled");
}

void TuningDeviceController::performImpact() {
    double currentAngle = freq2angle(mKey->getTunedFrequency());
    double targetAngle = freq2angle(mKey->getComputedFrequency());

    double error = currentAngle - targetAngle;

    // Check if already in target
    if (error < minErrorCents) {
        return;
    }

    // Check if too close
    if (calculateRequiredImpactVelocity(currentAngle, targetAngle) < getMinImpact()) {
        targetAngle = currentAngle + getMinImpact();
    }

    double velocity = calculateRequiredImpactVelocity(currentAngle, targetAngle);

    sendEngageCommand(velocity);
}

void TuningDeviceController::updateState(STATES state) {
    LogE("Update state not working...");
}

bool TuningDeviceController::sendEngageCommand(double velocity) {
    mImpactInProggress = true;
    updateState(TuningDeviceController::PERFORM_IMPACT);

    // Engange device through HTTP POST
    CurlEasy *curl = new CurlEasy;

    curl->set(CURLOPT_URL, DEVICE_ENGAGE_URL);
    curl->set(CURLOPT_TIMEOUT, 5L);

    // Set POST fields
    std::ostringstream data_ss;
    data_ss << "motor_speed=" << velocity;

    curl->set(CURLOPT_POSTFIELDS, data_ss.str().c_str());



    return true;
}

double TuningDeviceController::freq2angle(double frequency) {
    // TODO implement freq2angle mapping

    return 1;
}

double TuningDeviceController::calculateRequiredImpactVelocity(double currentAngle, double targetAngle) {
    // TODO implement mapping

    return 20;
}

double TuningDeviceController::getMinImpact() {
    return 10;
}


void TuningDeviceController::impactFinished() {
    mImpactInProggress = false;
    updateState(TuningDeviceController::AWAIT_KEYPRESS);
}

void TuningDeviceController::keyRecorded() {
    performImpact();
}
