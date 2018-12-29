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

#include "qtxmlreader.h"

#include <QFile>
#include <QTextStream>

#include "core/system/eptexception.h"
#include "core/system/log.h"

DECLARE_XML_READER_INTERFACE(QtXmlReader)

bool QtXmlReader::openFile(const std::wstring &path) {
    mReader.clear();

    QFile file(QString::fromStdWString(path));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        EPT_EXCEPT(EptException::ERR_CANNOT_READ_FROM_FILE, QString("Could not open file '%1'").arg(QString::fromStdWString(path)).toStdString().c_str());
    }

    mReader.setDevice(&file);
    if (mReader.hasError()) {
        EPT_EXCEPT(EptException::ERR_CANNOT_READ_FROM_FILE, QString("Could not create xml reader: %1").arg(mReader.errorString()).toStdString().c_str());
    }

    EptAssert(isOpen(), "Reader must be in open state");

    return true;
}

bool QtXmlReader::openString(const std::wstring &contents) {
    mReader.clear();

    mReader.addData(QString::fromStdWString(contents));

    if (mReader.hasError()) {
        EPT_EXCEPT(EptException::ERR_CANNOT_READ_FROM_FILE, QString("Could not create xml reader: %1").arg(mReader.errorString()).toStdString().c_str());
    }

    EptAssert(isOpen(), "Reader must be in open state");

    return true;
}

void QtXmlReader::clear() {
    mReader.clear();
    EptAssert(!isOpen(), "After clearing the reader must be in closed state.");
}

bool QtXmlReader::isOpen() {
    return mReader.hasError() == false;
}

bool QtXmlReader::atEnd() {
    EptAssert(isOpen(), "Reader must be open");
    return mReader.atEnd();
}

bool QtXmlReader::readNextStartElement() {
    EptAssert(isOpen(), "Reader must be open");

    return mReader.readNextStartElement();
}

bool QtXmlReader::isEndElement() {
    EptAssert(isOpen(), "Reader must be open");

    return mReader.isEndElement();
}

bool QtXmlReader::isStartElement() {
    EptAssert(isOpen(), "Reader must be open");

    return mReader.isStartElement();
}

bool QtXmlReader::readNext() {
    EptAssert(isOpen(), "Reader must be open");

    return mReader.readNext() != QXmlStreamReader::Invalid;
}

bool QtXmlReader::hasError() {
    return mReader.hasError();
}

std::string QtXmlReader::name() {
    EptAssert(isOpen(), "Reader must be open");

    return mReader.name().toString().toStdString();
}

bool QtXmlReader::hasAttribute(const std::string &id) {
    EptAssert(isOpen(), "Reader must be open");

    return mReader.attributes().hasAttribute(QString::fromStdString(id));
}

bool QtXmlReader::queryBoolAttribute(const std::string &id) {
    checkAttribute(id);

    QString t = mReader.attributes().value(QString::fromStdString(id)).toString().toLower();
    return t == "1" || t == "yes" || t == "y" || t == "true";
}

int QtXmlReader::queryIntAttribute(const std::string &id) {
    checkAttribute(id);

    bool b = false;

    int v = mReader.attributes().value(QString::fromStdString(id)).toInt(&b);

    if (!b) {
        std::string str_value = mReader.attributes().value(QString::fromStdString(id)).toString().toStdString();
        EPT_EXCEPT(EptException::ERR_CANNOT_READ_FROM_FILE, ("Attribute '" + id + "' with value '" + str_value + "' could not be parsed as type 'int'").c_str());
    }

    return v;
}

double QtXmlReader::queryRealAttribute(const std::string &id) {
    checkAttribute(id);

    bool b = false;

    double v = mReader.attributes().value(QString::fromStdString(id)).toDouble(&b);

    if (!b) {
        std::string str_value = mReader.attributes().value(QString::fromStdString(id)).toString().toStdString();
        EPT_EXCEPT(EptException::ERR_CANNOT_READ_FROM_FILE, ("Attribute '" + id + "' with value '" + str_value + "' could not be parsed as type 'double'").c_str());
    }

    return v;
}

std::wstring QtXmlReader::queryWStringAttribute(const std::string &id) {
    checkAttribute(id);

    return mReader.attributes().value(QString::fromStdString(id)).toString().toStdWString();
}

std::string QtXmlReader::queryStringAttribute(const std::string &id) {
    checkAttribute(id);

    return mReader.attributes().value(QString::fromStdString(id)).toString().toStdString();
}

std::wstring QtXmlReader::queryWStringText() {
    return mReader.readElementText().toStdWString();
}

std::string QtXmlReader::queryStringText() {
    return mReader.readElementText().toStdString();
}

std::vector<double> QtXmlReader::queryDoubleVectorText() {
    std::vector<double> out;

    QString s = mReader.readElementText();

    QTextStream ts(&s, QIODevice::ReadOnly);
    ts.setLocale(QLocale(QLocale::English));

    double v;
    while (ts.atEnd() == false) {
      ts >> v;
      out.push_back(v);
    }

    return out;
}

double QtXmlReader::queryDoubleText() {
    return mReader.readElementText().toDouble();
}

int QtXmlReader::queryIntText() {
    return mReader.readElementText().toInt();
}
