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

#ifndef QTXMLREADER_H
#define QTXMLREADER_H

#include <QXmlStreamReader>

#include "core/adapters/xmlreaderinterface.h"

class QtXmlReader : public XmlReaderInterface
{
public:
    virtual bool openFile(const std::wstring &path) override final;
    virtual bool openString(const std::wstring &contents) override final;
    virtual void clear() override final;

    virtual bool isOpen() override final;

    virtual bool atEnd() override final;
    virtual bool readNextStartElement() override final;
    virtual bool isEndElement() override final;
    virtual bool isStartElement() override final;
    virtual bool readNext() override final;
    virtual bool hasError() override final;
    virtual std::string name() override final;

    virtual bool hasAttribute(const std::string &id) override final;

    virtual bool queryBoolAttribute(const std::string &id) override final;
    virtual int queryIntAttribute(const std::string &id) override final;
    virtual double queryRealAttribute(const std::string &id) override final;
    virtual std::wstring queryWStringAttribute(const std::string &id) override final;
    virtual std::string queryStringAttribute(const std::string &id) override final;

    virtual std::wstring queryWStringText() override final;
    virtual std::string queryStringText() override final;
    virtual std::vector<double> queryDoubleVectorText() override final;
    virtual double queryDoubleText() override final;
    virtual int queryIntText() override final;


private:
    QXmlStreamReader mReader;
};

#endif // QTXMLREADER_H
