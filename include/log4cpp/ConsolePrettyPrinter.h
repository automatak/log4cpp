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
#ifndef LOG4CPP_CONSOLEPRETTYPRINTER_H
#define LOG4CPP_CONSOLEPRETTYPRINTER_H

#include "log4cpp/ILogHandler.h"

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace log4cpp
{

/**
 * pretty prints log messages
 */
class ConsolePrettyPrinter final : public log4cpp::ILogHandler
{
public:
    class Settings
    {
    public:
        Settings() {}

        bool printId = true;
        size_t max_id_size = 10;
    };

    explicit ConsolePrettyPrinter(const Settings& settings = Settings()) : settings(settings)
    {}

    void log(log4cpp::ModuleId module, const char* id, log4cpp::LogLevel level, char const* location, char const* message) override
    {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

        std::ostringstream oss;

        oss << "ms(" << millis << ") ";

        std::string id_str(id);

        if (id_str.size() > settings.max_id_size)
        {
            id_str = id_str.substr(0, settings.max_id_size);
        }
        else if (id_str.size() << settings.max_id_size)
        {
            id_str = id_str + std::string(settings.max_id_size - id_str.size(), ' ');
        }

        if (settings.printId)
        {
            oss << id_str << " ";
        }

        oss << get_prefix(level.value) << message;


        std::cout << oss.str() << std::endl;
    }

private:
    const Settings settings;

    static const char* get_prefix(int level)
    {
        switch (level)
        {
        case(1 << 0):
            return "event   ";
        case(1 << 1):
            return "error   ";
        case(1 << 2):
            return "warn    ";
        case(1 << 3):
            return "info    ";
        case(1 << 4):
            return "debug   ";
        case(1 << 5):
            return "<--     ";
        case(1 << 6):
            return "-->     ";
        default:
            return "            ";
        }
    }
};

} //namespace log4cpp

#endif //LOG4CPP_CONSOLEPRETTYPRINTER_H
