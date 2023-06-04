
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : Logger.cc
*   Last Modified : 2019-11-04 19:09
*   Describe      :
*
*******************************************************/

#include "Logger.h"

const char* LevelName[NUM_LEVELS] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR"
};

Logger* Logger::s_instance = nullptr;
std::mutex Logger::s_mtx;
LEVEL Logger::s_logLevel = INFO;

static std::string getTimeStr() {
    auto t = std::chrono::system_clock::now();
    auto as_time_t = std::chrono::system_clock::to_time_t(t);

    std::chrono::steady_clock::duration d = t.time_since_epoch();
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(d);
    
	struct tm tm;
	::gmtime_r(&as_time_t, &tm);

    char buf[80];
	if(::strftime(buf, sizeof buf, "%F %H:%M:%S", &tm))
        sprintf(buf, "%s.%03ld", buf, ms.count()%1000);

	return std::string(buf);
}

Logger* Logger::getInstance() {
    if(s_instance == nullptr) {
        std::lock_guard<std::mutex> lg(s_mtx);
        if(s_instance == nullptr) {
            Logger* tmp = new Logger();
            s_instance = tmp;
        }
    }
    return s_instance;
}

void Logger::destroyInstance() {
    if(s_instance != nullptr) {
        delete s_instance;
        s_instance = nullptr;
    }
}

void Logger::setLogLevel(LEVEL logLevel) {
    s_logLevel = logLevel;
}

LEVEL Logger::getLogLevel() {
    return s_logLevel;
}

void Logger::setLogFile(const std::string& fileName) {
    if(!fileName.empty()) {
        _ofs.open(fileName, std::ios::app|std::ios::out); // 生成空文件或者在文件尾追加
        if(_ofs.fail()) {
            printf("Failed to open logfile.");
        }
    } 
}

void Logger::logFile(LEVEL logLevel, 
		             const std::string& fileName, 
		             unsigned int line, 
		             const std::string& info) {
    char buf[512];
    sprintf(buf, "%s %-5s %s:%d - %s",
            getTimeStr().c_str(), LevelName[logLevel],
            fileName.c_str(), line, 
            info.c_str());

    std::string entry(buf);
    {
        std::lock_guard<std::mutex> lg(_mtx);
        _queue.push(std::move(entry));   // 减少拷贝
    }
    _cv.notify_one();
}

Logger::Logger() { 
    _stop = false;
    _thread = std::thread(std::bind(&Logger::work, this));
}

Logger::~Logger() {
    _stop = true;
    _cv.notify_one();
    _thread.join();
    
    if(_ofs.is_open())
        _ofs.close();
}

void Logger::work() {
    while(!_stop) {
        std::string entry;
        {
            std::unique_lock<std::mutex> ul(_mtx);
            _cv.wait(ul, [this] { return _stop || !_queue.empty(); });
            
            if(_stop) break;
            entry = _queue.front();
            _queue.pop();
        }

        if(_ofs.is_open()) {
            _ofs << entry << std::endl;
        }
        else {
            printf("%s\n", entry.c_str());
        }
    }

    // 退出前打印队列剩余日志信息
    while(!_queue.empty()) {
        std::string entry = _queue.front();
        _queue.pop();
        
        if(_ofs.is_open()) {
            _ofs << entry << std::endl;
        }
        else {
            printf("%s\n", entry.c_str());
        }
    }
}

