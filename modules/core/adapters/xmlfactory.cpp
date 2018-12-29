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

#include "xmlfactory.h"

#include "core/system/eptexception.h"

XmlFactory::XmlFactory()
{

}

XmlFactory *XmlFactory::getSingletonPtr() {
    static XmlFactory mSingleton;
    return &mSingleton;
}

XmlReaderInterfacePtr XmlFactory::getDefaultReader() {
    if (getSingletonPtr()->mReaderInterfaces.size() == 0) {
        EPT_EXCEPT(EptException::ERR_INTERNAL_ERROR, "No xml reader has been registered!");
    }

    return getSingletonPtr()->mReaderInterfaces.front()->createInstance();
}

XmlWriterInterfacePtr XmlFactory::getDefaultWriter() {
    if (getSingletonPtr()->mWriterInterfaces.size() == 0) {
        EPT_EXCEPT(EptException::ERR_INTERNAL_ERROR, "No xml writer has been registered!");
    }

    return getSingletonPtr()->mWriterInterfaces.front()->createInstance();
}
