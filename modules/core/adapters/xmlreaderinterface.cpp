#include "xmlreaderinterface.h"

#include "xmlfactory.h"

XmlReaderInterface::~XmlReaderInterface() {

}

XmlReaderFactoryBase::XmlReaderFactoryBase(const std::string &id) :
  mId(id) {
    XmlFactory::getSingletonPtr()->registerReader(this);
}

XmlReaderFactoryBase::~XmlReaderFactoryBase() {

}
