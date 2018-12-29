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

#ifndef ALGORITHMPLUGIN_H
#define ALGORITHMPLUGIN_H

#include "core/config.h"

#include "algorithmfactorydescription.h"
#include "core/calculation/algorithm.h"
#include "core/calculation/algorithmfactory.h"
#include "core/messages/messagelistener.h"

// Define the API version
// This value shall be incremented whenever there are API breaking changes.
#define ALGORITHM_PLUGIN_API_VERSION 1

#ifdef WIN32
#  define ALGORITHM_PLUGIN_EXPORT __declspec(dllexport)
#else
#  define ALGORITHM_PLUGIN_EXPORT  // empty
#endif

#define ALGORITHM_STRINGIFY(x) #x

// Define a type for the static function pointer.
class AlgorithmFactoryBase;
typedef AlgorithmFactoryBase* (*GetAlgorithmFactoryFunc)();

// Algorithm plugin details structure that is exposed to the application
struct AlgorithmPluginDetails {
  int apiVersion;
  const char* fileName;
  const char* className;
  AlgorithmFactoryDescription description;
  GetAlgorithmFactoryFunc factoryFunc;
};

// ================================================================================
// Header defines

// general
#define ALGORITHM_H_START(algorithmName)                                \
    ALGORITHM_INIT_RESOURCE_FUNC(algorithmName)                         \
    namespace algorithmName {                                           \
                                                                        \
    ALGORITHM_PLUGIN_EXPORT std::string getAlgorithmVersion();          \
                                                                        \
    inline AlgorithmFactoryDescription getFactoryDescription() {        \
        return AlgorithmFactoryDescription(#algorithmName, getAlgorithmVersion()); \
    }                                                                   \
                                                                        \
    inline AlgorithmFactoryDescription getInitFactoryDescription() {    \
        ALGORITHM_INIT_FUNC_NAME(algorithmName)                         \
        return getFactoryDescription();                                 \
    }                                                                   \
                                                                        \

#define ALGORITHM_H_END(Class)                                          \
    typedef AlgorithmFactory<Class> Factory; }


// static extra defines
#if defined(EPT_STATIC_ALGORITHMS)

// we need QResource for Q_INIT_RESOURCE
#  include <QResource>
#  define ALGORITHM_INIT_RESOURCE_FUNC(algorithmName)                   \
    inline void initResource##algorithmName() {                         \
        Q_INIT_RESOURCE(algorithmName);                                 \
    }                                                                   \

#  define ALGORITHM_INIT_FUNC_NAME(algorithmName)                       \
    initResource##algorithmName();                                      \

#endif


// ==================================================================================
// CPP defines

// general implementations (must be included in namespace)
#define ALGORITHM_DECLARATIONS                                          \
    std::string getAlgorithmVersion() {                                 \
        return EPT_ALGORITHM_VERSION_NAME;                              \
    }                                                                   \

#if defined(EPT_STATIC_ALGORITHMS)

// start
#  define ALGORITHM_CPP_START(algorithmName)                          \
    namespace algorithmName {                                         \
        ALGORITHM_DECLARATIONS

// end
#  define ALGORITHM_CPP_END }

#else
#  define ALGORITHM_INIT_RESOURCE_FUNC(algorithmName)
#  define ALGORITHM_INIT_FUNC_NAME(algorithmName)


// start
//
// Notes: Do not auto register the factory, be cause of conflicts with
//        static variables being not shared between lib and core
//        Instead add the factory during the loading in CalculationManager
#  define ALGORITHM_CPP_START(algorithmName)                          \
    extern "C" {                                                      \
        ALGORITHM_PLUGIN_EXPORT AlgorithmFactoryBase *getFactory()    \
        {                                                             \
            static algorithmName::Factory mSingleton(algorithmName::getFactoryDescription(), false);       \
            return &mSingleton;                                       \
        }                                                             \
        ALGORITHM_PLUGIN_EXPORT AlgorithmPluginDetails exports =      \
        {                                                             \
            ALGORITHM_PLUGIN_API_VERSION,                             \
            __FILE__,                                                 \
            #algorithmName,                                           \
            algorithmName::getFactoryDescription(),                   \
            getFactory                                                \
        };                                                            \
    }                                                                 \
    namespace algorithmName {                                         \
        ALGORITHM_DECLARATIONS


// end
#  define ALGORITHM_CPP_END }
#endif


#endif // ALGORITHMPLUGIN_H
