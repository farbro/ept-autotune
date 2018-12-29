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

#ifndef XMLWRITERINTERFACE_H
#define XMLWRITERINTERFACE_H

#include "prerequisites.h"

class EPT_EXTERN XmlWriterInterface
{
public:
    virtual bool open() = 0;
    virtual std::wstring close() = 0;

    virtual bool hasError() = 0;

    virtual void writeStartDocument() = 0;
    virtual void writeEndDocument() = 0;
    virtual void writeStartElement(const std::string &name) = 0;
    virtual void writeEndElement() = 0;

    virtual void writeAttribute(const std::string &name, const double &value) = 0;
    virtual void writeAttribute(const std::string &name, const int &value) = 0;
    void writeAttribute(const std::string &name, const char *value);
    virtual void writeAttribute(const std::string &name, const std::string &value) = 0;
    virtual void writeAttribute(const std::string &name, const std::wstring &value) = 0;
    virtual void writeAttribute(const std::string &name, const bool &value) = 0;

    virtual void writeTextElement(const std::string &name, const std::string &value) = 0;
    virtual void writeTextElement(const std::string &name, const std::wstring &value) = 0;

    virtual void writeDoubleListElement(const std::string &name, const std::vector<double> &value, const std::string &separator = " ") = 0;

    virtual void writeCharacters(const double &value) = 0;
    virtual void writeCharacters(const std::string &value) = 0;
    virtual void writeCharacters(const std::wstring &value) = 0;
};

typedef std::shared_ptr<XmlWriterInterface> XmlWriterInterfacePtr;

class EPT_EXTERN XmlWriterFactoryBase {
protected:
    XmlWriterFactoryBase(const std::string &mId);

public:
    virtual ~XmlWriterFactoryBase() {}
    virtual XmlWriterInterfacePtr createInstance() = 0;

    const std::string &getId() const {return mId;}

private:
    const std::string mId;
};

template <class T>
class XmlWriterFactory : private XmlWriterFactoryBase {
protected:
    XmlWriterFactory(const std::string &id) : XmlWriterFactoryBase(id) {}

    XmlWriterInterfacePtr createInstance() override final {
        return XmlWriterInterfacePtr(new T);
    }

private:
    static XmlWriterFactory mSingleton;
};

#define DECLARE_XML_WRITER_INTERFACE(clazz) \
    template <> XmlWriterFactory<clazz> XmlWriterFactory<clazz>::mSingleton(#clazz);


#endif // XMLWRITERINTERFACE_H
