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

#include "qtpcmdevice.h"
#include "audioplayerforqt.h"

QtPCMDevice::QtPCMDevice(AudioInterfaceForQt *audioInterface)
    : mAudioInterface(audioInterface) {
    EptAssert(audioInterface, "A valid audio interface is required");
}

bool QtPCMDevice::open(OpenMode mode) {
    if (mDevice) {
        mDevice->open(mAudioInterface);
    }
    return QIODevice::open(mode);
}

void QtPCMDevice::close() {
    if (mDevice) {
        mDevice->close();
    }

    QIODevice::close();
}

void QtPCMDevice::setDevice(PCMDevice *device) {
    if (isOpen() && mDevice) {
        mDevice->close();
    }
    mDevice = device;
    if (isOpen() && mDevice) {
        mDevice->open(mAudioInterface);
    }
}

qint64 QtPCMDevice::readData(char *data, qint64 maxSize) {
    if (mDevice) {
        return mDevice->read(data, maxSize);
    }

    std::fill(data, data + maxSize, 0);
    return maxSize;
}

qint64 QtPCMDevice::writeData(const char *data, qint64 maxSize) {
    if (mDevice) {
        return mDevice->write(data, maxSize);
    }

    return 0;
}
