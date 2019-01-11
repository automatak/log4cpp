/*
 * Copyright (c) 2019, Automatak LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef LOG4CPP_LOGLEVELS_H
#define LOG4CPP_LOGLEVELS_H

#include <cstdint>

namespace log4cpp
{

struct ModuleId
{
public:
    ModuleId() = default;

    explicit ModuleId(int32_t level) : value(level)
    {}

    int32_t	value = 0;
};

struct LogLevel
{
public:
    LogLevel() = default;

    explicit LogLevel(int32_t level) : value(level)
    {}

    LogLevel next() const
    {
        return LogLevel(value << 1);
    }

    int32_t	value = 0;
};

// some default log flags
namespace levels
{
    const LogLevel event = LogLevel(1);
    const LogLevel error = event.next();
    const LogLevel warn = error.next();
    const LogLevel info = warn.next();
    const LogLevel debug = info.next();
}

/**
 * Strongly typed wrapper for flags bitfield
 */
class LogLevels
{
public:
    LogLevels() = default;

    explicit LogLevels(int32_t levels) : levels(levels)
    {}

    static LogLevels everything()
    {
        return LogLevels(~0);
    }

    inline bool is_set(const LogLevel& level) const
    {
        return (level.value & levels) != 0;
    }

    LogLevels& operator|=(const LogLevels& other)
    {
        this->levels |= other.levels;
        return *this;
    }

    LogLevels operator|(const LogLevels& other) const
    {
        return LogLevels(this->levels | other.levels);
    }

    inline int32_t get_value() const
    {
        return levels;
    }

private:
    int32_t	levels = 0;
};

} //namespace log4cpp

#endif //LOG4CPP_LOGLEVELS_H
