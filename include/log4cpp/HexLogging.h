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
#ifndef LOG4CPP_HEXLOGGING_H
#define LOG4CPP_HEXLOGGING_H

#include "ser4cpp/container/SequenceTypes.h"
#include "ser4cpp/util/HexConversions.h"
#include "ser4cpp/util/Uncopyable.h"

#include "log4cpp/Location.h"
#include "log4cpp/Logger.h"

namespace log4cpp
{

class HexLogging : private ser4cpp::StaticOnly
{
    static const uint32_t max_hex_per_line = max_log_entry_size / 3;

public:
    static void log(
        Logger& logger,
        LogLevel level,
        const ser4cpp::rseq_t& source,
        char separator = ' ',
        uint32_t first_row_size = max_hex_per_line,
        uint32_t other_row_size = max_hex_per_line
    )
    {
        ser4cpp::rseq_t copy(source);
        uint32_t row = 0;

        const auto max_first_size = ser4cpp::bounded<uint32_t>(first_row_size, 1, max_hex_per_line);
        const auto max_other_size = ser4cpp::bounded<uint32_t>(other_row_size, 1, max_hex_per_line);

        while (copy.is_not_empty())
        {
            const auto row_size = (row == 0) ? max_first_size : max_other_size;
            copy = log_line(logger, level, copy, separator, row_size);
            ++row;
        }
    }

private:
    static ser4cpp::rseq_t log_line(
        Logger& logger,
        LogLevel level,
        const ser4cpp::rseq_t& data,
        char separator,
        uint32_t max_row_size
    )
    {
        char buffer[max_log_entry_size];

        uint32_t count = 0;

        while ((count < max_row_size) && (count < data.length()))
        {
            auto pos = count * 3;
            buffer[pos] = ser4cpp::HexConversions::to_hex_char((data[count] & 0xF0) >> 4);
            buffer[pos + 1] = ser4cpp::HexConversions::to_hex_char(data[count] & 0x0F);
            buffer[pos + 2] = separator;
            ++count;
        }

        buffer[(3 * count) - 1] = '\0';

        logger.log(level, LOCATION, buffer);

        return data.skip(count);
    }
};

} //namespace log4cpp

#endif //LOG4CPP_HEXLOGGING_H
