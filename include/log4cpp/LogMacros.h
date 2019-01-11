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
#ifndef LOG4CPP_LOGMACROS_H
#define LOG4CPP_LOGMACROS_H

#include "log4cpp/Location.h"
#include "log4cpp/HexLogging.h"

#include <cstdio>

#ifdef WIN32
#define SAFE_STRING_FORMAT(dest, length_, format, ...) _snprintf_s(dest, length_, _TRUNCATE, format, ##__VA_ARGS__)
#else
#define SAFE_STRING_FORMAT(dest, size, format, ...) snprintf(dest, size, format, ##__VA_ARGS__)
#endif //WIN32

#define LOG_FORMAT(logger, levels, format, ...) { \
	char message_buffer_some_name_no_conflict[log4cpp::max_log_entry_size]; \
	SAFE_STRING_FORMAT(message_buffer_some_name_no_conflict, log4cpp::max_log_entry_size, format, ##__VA_ARGS__); \
	logger.log(levels, LOCATION, message_buffer_some_name_no_conflict); } \
 
#define SIMPLE_LOG_BLOCK(logger, levels, message) \
		if(logger.is_enabled(levels)){ \
			logger.log(levels, LOCATION, message); \
		}

#define SIMPLE_LOGGER_BLOCK(pLogger, levels, message) \
		if(pLogger && pLogger->is_enabled(levels)){ \
			pLogger->log(levels, LOCATION, message); \
		}

#define FORMAT_LOG_BLOCK(logger, levels, format, ...) \
	if(logger.is_enabled(levels)){ \
		char message_buffer_some_name_no_conflict[log4cpp::max_log_entry_size]; \
		SAFE_STRING_FORMAT(message_buffer_some_name_no_conflict, log4cpp::max_log_entry_size, format, ##__VA_ARGS__); \
		logger.log(levels, LOCATION, message_buffer_some_name_no_conflict); \
	}

#define FORMAT_LOGGER_BLOCK(pLogger, levels, format, ...) \
	if(pLogger && pLogger->is_enabled(levels)){ \
		char message_buffer_some_name_no_conflict[log4cpp::max_log_entry_size]; \
		SAFE_STRING_FORMAT(message_buffer_some_name_no_conflict, log4cpp::max_log_entry_size, format, ##__VA_ARGS__); \
		pLogger->log(levels, LOCATION, message_buffer_some_name_no_conflict); \
	}

#endif //LOG4CPP_LOGMACROS_H
