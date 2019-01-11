cmake_minimum_required (VERSION 3.8)

project(log4cpp VERSION 1.0.0)

option(LOG4CPP_SUPPRESS_LOG_LOCATION "Do not print source location in logs" OFF)

set(log4cpp_public_headers
    ./include/log4cpp/HexLogging.h
    ./include/log4cpp/ILogHandler.h
    ./include/log4cpp/Location.h
    ./include/log4cpp/Logger.h
    ./include/log4cpp/LogLevels.h
    ./include/log4cpp/LogMacros.h
    ./include/log4cpp/MockLogHandler.h
    ./include/log4cpp/Strings.h
)

add_library(log4cpp INTERFACE)
target_link_libraries(log4cpp INTERFACE ser4cpp)
target_include_directories(log4cpp INTERFACE ./include)
target_compile_features(log4cpp INTERFACE cxx_std_11)

if(${LOG4CPP_SUPPRESS_LOG_LOCATION})
    target_compile_definitions(log4cpp INTERFACE LOG4CPP_SUPPRESS_LOG_LOCATION)
endif()
