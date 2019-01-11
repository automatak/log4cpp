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
#ifndef LOG4CPP_LOGGER_H
#define LOG4CPP_LOGGER_H

#include "log4cpp/ILogHandler.h"
#include "log4cpp/Strings.h"

#include <memory>
#include <string>

namespace log4cpp
{

const uint32_t max_log_entry_size = 120;

/**
 * A copyable facade over a LogRoot class
 */
class Logger
{
public:
	struct Settings
	{
		Settings(ModuleId module, const std::string& id, LogLevels levels) : module(module), id(id), levels(levels)
		{}

		ModuleId module;
		std::string id;
		LogLevels levels;
	};

	Logger(const std::shared_ptr<ILogHandler>& backend, ModuleId moduleid, const std::string& id, LogLevels levels) :
		backend(backend),
		settings(std::make_shared<Settings>(moduleid, id, levels))
	{}

	static Logger empty()
	{
		return Logger(nullptr, ModuleId(0), "", LogLevels(0));
	}

	void log(const LogLevel& level, const char* location, const char* message)
	{
		if (backend)
		{
			backend->log(	
					this->settings->module,
					this->settings->id.c_str(),
					level,
					location,
					message					
			);
		}
	}

	Logger detach(const std::string& id) const
	{
		return Logger(this->backend, std::make_shared<Settings>(this->settings->module, id, this->settings->levels));
	}

	template <typename... Args>
	Logger detach_and_append(Args... args) const
	{			
		return detach(Strings::concatenate(this->settings->id, args...));
	}

	Logger detach(const std::string& id, LogLevels levels) const
	{
		return Logger(this->backend, std::make_shared<Settings>(this->settings->module, id, levels));
	}

	Logger detach(LogLevels levels) const
	{
		return Logger(this->backend, std::make_shared<Settings>(this->settings->module, this->settings->id, levels));
	}

	bool is_enabled(const LogLevel& level) const
	{
		return backend && settings->levels.is_set(level);
	}

	LogLevels get_levels() const
	{
		return this->settings->levels;
	}

	void set_levels(const LogLevels& filters)
	{
		this->settings->levels = filters;
	}

	void rename(const std::string& id)
	{
		this->settings->id = id;
	}

private:
	Logger(const std::shared_ptr<ILogHandler>& backend, const std::shared_ptr<Settings>& settings) :
		backend(backend),
		settings(settings)
	{}

	Logger() = delete;
	Logger& operator=(const Logger&) = delete;

	const std::shared_ptr<ILogHandler> backend;
	const std::shared_ptr<Settings> settings;
};

} //namespace log4cpp

#endif //LOG4CPP_LOGGER_H
