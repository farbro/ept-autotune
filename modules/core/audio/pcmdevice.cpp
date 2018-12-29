/*****************************************************************************
 * Copyright 2018 Haye Hinrichsen, Christoph Wick
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

#include "pcmdevice.h"
#include "audiointerface.h"
#include "core/system/eptexception.h"
#include "core/system/log.h"

void PCMDevice::open(AudioInterface *audioInterface)
{
    EptAssert(audioInterface, "Audio interface is nullptr");

    mAudioInterface = audioInterface;
}

void PCMDevice::close()
{
    mAudioInterface = nullptr;
}

int PCMDevice::getSampleRate() const {
    if (mAudioInterface) {
        return mAudioInterface->getSamplingRate();
    }

    LogE("Requested sampling rate though device is not open.");
    return 0;
}

int PCMDevice::getChannels() const {
    if (mAudioInterface) {
        return mAudioInterface->getChannelCount();
    }

    LogE("Requested channel count though device is not open.");

    return 1;
}
