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

#ifndef MESSAGESIGNALANALYSIS_H
#define MESSAGESIGNALANALYSIS_H

#include "message.h"

///////////////////////////////////////////////////////////////////////////////
/// \brief Message sent whenever the data associated with a single key changes.
///////////////////////////////////////////////////////////////////////////////

class MessageSignalAnalysis : public Message
{
public:
    enum class Status {
        STARTED,
        ENDED
    };

    enum class Result {
        SUCCESSFULL,
        INVALID
    };

    MessageSignalAnalysis(Status status, Result result = Result::SUCCESSFULL, int invalidAttempts = -1)
        : Message(Message::MSG_SIGNAL_ANALYSIS)
        , mStatus(status)
        , mResult(result)
        , mInvalidAttempts(invalidAttempts) {

    }

    Status status() const {return mStatus;}
    Result result() const {return mResult;}
    int invalidAttempts() const {return mInvalidAttempts;}

private:
    const Status mStatus;
    const Result mResult;
    const int mInvalidAttempts;

};

#endif // MESSAGESIGNALANALYSIS_H
