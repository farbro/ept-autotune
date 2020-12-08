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

class Core;

class TuningDeviceController
{

public:

    enum STATES {
        IDLE,
        AWAIT_PRE_TONE,
        TUNE,
        AWAIT_POST_TONE
    };

    TuningDeviceController(Core *core);
    virtual ~TuningDeviceController() {}

    void startTuning(const Key &key);
    void stopTuning();

    void runTuningCycle(Key key, double setValue, bool automatic);
    void performImpact(double value);
    STATES getState();

private:
    Core *mCore;

    STATES state;
    double minErrorCents = 10;
    Key key;

    double nextSetValue(Key key);
    double error(Key key);
    void tune();
};

#endif // TUNINGDEVICECONTROLLER_H
