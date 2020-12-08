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

TuningDeviceController::TuningDeviceController(Core *core) : mCore(core) {
}

void TuningDeviceController::startTuning(const Key &key) {
    Piano  &piano = mCore->getPianoManager()->getPiano();
    LogI("Start tuning key to %f Hz", key.getComputedFrequency());
    //piano.getKeyboard().getKeyNumberOfA4()
}

void TuningDeviceController::stopTuning() {
    LogI("Stopping tuning");
}

void TuningDeviceController::runTuningCycle(Key key, double setValue, bool automatic) {

}
void TuningDeviceController::performImpact(double value) {

}

TuningDeviceController::STATES TuningDeviceController::getState() {
    return state;
}
