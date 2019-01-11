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
#ifndef LOG4CPP_MOCKLOGHANDLER_H
#define LOG4CPP_MOCKLOGHANDLER_H

#include "log4cpp/Logger.h"
#include "ser4cpp/util/Uncopyable.h"

#include "catch.hpp"

#include <deque>
#include <iostream>

namespace log4cpp
{
    class MockLogHandler : public ILogHandler, ser4cpp::Uncopyable
    {
        struct Backend final : public ILogHandler
        {
            void log(ModuleId module, const char* id, LogLevel level, char const* location, char const* message) override
            {
                if (output_to_stdio)
                {
                    std::cout << message << std::endl;
                }

                lines.push_back(message);
            }

            bool output_to_stdio = false;
            std::deque<std::string> lines;
        };

    public:
        MockLogHandler(const std::string& id, LogLevels levels = LogLevels::everything()) :
            backend(std::make_shared<Backend>()),
            logger(backend, ModuleId(0), id, levels)
        {}

        void print_output()
        {
            this->backend->output_to_stdio = true;
        }

        virtual void log(ModuleId module, const char* id, LogLevel level, char const* location, char const* message) override
        {
            this->backend->log(module, id, level, location, message);
        }

        template <typename... Args>
        void expect(const std::string& expected, const Args& ... args)
        {
            REQUIRE_FALSE(backend->lines.empty());
            REQUIRE(expected == backend->lines.front());
            backend->lines.pop_front();
            expect(args ...);
        }

        void expect()
        {
            REQUIRE(backend->lines.empty());
        }

    private:
        std::shared_ptr<Backend> backend;

        MockLogHandler() = delete;

    public:
        log4cpp::Logger logger;
    };

} //namespace log4cpp

#endif //LOG4CPP_MOCKLOGHANDLER_H
