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

#include "sharedlibrary.h"
#include "core/system/eptexception.h"

#ifdef EPT_SHARED_ALGORITHMS

SharedLibrary::SharedLibrary(const std::string &path) :
    mOpen(false),
    mPath(path) {
    mOpen = open(path);
}

SharedLibrary::~SharedLibrary() {
    close();
}

bool SharedLibrary::open(const std::string &path) {

    if (uv_dlopen(path.c_str(), &mLib)) {
        LogW("Could not load dynamic library %s, error: %s", path.c_str(), uv_dlerror(&mLib));
        return false;
    }

    LogI("SharedLibrary '%s' opened.", mPath.c_str());

    return true;
}

void SharedLibrary::close() {
    if (!mOpen) {return;}

    uv_dlclose(&mLib);
    LogI("SharedLibrary '%s' closed.", mPath.c_str());
}

bool SharedLibrary::sym(const char *name, void **ptr) {
    EptAssert(mOpen, "Library has to be open before calling sym.");

    if (uv_dlsym(&mLib, name, ptr)) {
        LogW("Symbol '%s' not found, error: %s", name, uv_dlerror(&mLib));
        return false;
    }
    return true;
}

#endif
