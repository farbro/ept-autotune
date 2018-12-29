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

#ifndef QTXMLWRITER_H
#define QTXMLWRITER_H

#include <QXmlStreamWriter>

#include "core/adapters/xmlwriterinterface.h"

class QtXmlWriter : public XmlWriterInterface
{
public:
    QtXmlWriter();
    virtual ~QtXmlWriter();

    virtual bool open() override final;
    virtual std::wstring close() override final;

    virtual bool hasError() override final;

    virtual void writeStartDocument() override final;
    virtual void writeEndDocument() override final;
    virtual void writeStartElement(const std::string &name) override final;
    virtual void writeEndElement() override final;

    virtual void writeAttribute(const std::string &name, const double &value) override final;
    virtual void writeAttribute(const std::string &name, const int &value) override final;
    virtual void writeAttribute(const std::string &name, const std::string &value) override final;
    virtual void writeAttribute(const std::string &name, const std::wstring &value) override final;
    virtual void writeAttribute(const std::string &name, const bool &value) override final;

    virtual void writeTextElement(const std::string &name, const std::string &value) override final;
    virtual void writeTextElement(const std::string &name, const std::wstring &value) override final;

    virtual void writeDoubleListElement(const std::string &name, const std::vector<double> &value, const std::string &separator = " ") override final;

    virtual void writeCharacters(const double &value) override final;
    virtual void writeCharacters(const std::string &value) override final;
    virtual void writeCharacters(const std::wstring &value) override final;

private:
    QString mString;
    QXmlStreamWriter mWriter;
};

#endif // QTXMLWRITER_H
