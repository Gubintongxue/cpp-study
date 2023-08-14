
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : Logger.h
*   Last Modified : 2019-11-04 19:06
*   Describe      :
*
*******************************************************/

#ifndef  _LOGGER_H
#define  _LOGGER_H

#include <cstdio>
#include <cstring>
#include <cstdarg>

#include <fstream>
#include <memory>
#include <string>
#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <functional> //without .h


#define TRIM(x) strrchr(x,'/')? strrchr(x,'/')+1 : x

enum LEVEL {
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
    NUM_LEVELS
};

class Logger {
public:
    static Logger* getInstance();
    static void destroyInstance();

    static void setLogLevel(LEVEL logLevel);
    static LEVEL getLogLevel();

    void setLogFile(const std::string& fileName = "");

    void logFile(LEVEL logLevel, 
                 const std::string& fileName, 
                 unsigned int line, 
                 const std::string& info);

private:
    Logger();
    ~Logger();
    Logger(const Logger&);
    Logger& operator=(const Logger&);
    
    void work();

private:
    static Logger* s_instance;
    static std::mutex s_mtx;
    static LEVEL s_logLevel;

    std::atomic<bool> _stop;
    std::mutex _mtx;
    std::condition_variable _cv;
    std::thread _thread;
    std::queue<std::string> _queue;
    std::ofstream _ofs;
};

inline void sprintfBuffer(char* buffer, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buffer, fmt, ap);
    va_end(ap);
}

#define LOG_INIT(fileName) \
    Logger::getInstance()->setLogFile(fileName); \

#define LOG_UNINIT() \
    Logger::destroyInstance();

#define LOG_LEVEL(logLevel) \
    Logger::setLogLevel(logLevel);

#define LOG(logLevel, ...) \
    if(Logger::getInstance()->getLogLevel() <= logLevel) { \
        char info[512]; \
        sprintfBuffer(info, __VA_ARGS__); \
        Logger::getInstance()->logFile(logLevel, TRIM(__FILE__), __LINE__, info); \
    }

#endif // _LOGGER_H
