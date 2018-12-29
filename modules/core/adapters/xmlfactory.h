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

#ifndef XMLFACTORY_H
#define XMLFACTORY_H

#include "prerequisites.h"

#include "xmlreaderinterface.h"
#include "xmlwriterinterface.h"

EPT_EXTERN_TEMPLATE class std::vector<XmlReaderFactoryBase*>;
EPT_EXTERN_TEMPLATE class std::vector<XmlWriterFactoryBase*>;

class EPT_EXTERN XmlFactory
{
private:
    XmlFactory();

public:
    static XmlFactory *getSingletonPtr();

    void registerReader(XmlReaderFactoryBase *reader) {mReaderInterfaces.push_back(reader);}
    void registerWriter(XmlWriterFactoryBase *writer) {mWriterInterfaces.push_back(writer);}

    static XmlReaderInterfacePtr getDefaultReader();
    static XmlWriterInterfacePtr getDefaultWriter();

private:
    std::vector<XmlReaderFactoryBase*> mReaderInterfaces;
    std::vector<XmlWriterFactoryBase*> mWriterInterfaces;
};

#endif // XMLFACTORY_H
