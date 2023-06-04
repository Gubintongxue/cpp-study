
/******************************************************
*   Copyright (C)2019 All rights reserved.
*
*   Author        : owb
*   Email         : 2478644416@qq.com
*   File Name     : ThreadPool.h
*   Last Modified : 2019-10-11 12:50
*   Describe      :
*
*******************************************************/

#ifndef  _THREADPOOL_H
#define  _THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
    typedef std::function<void()> Task;

public:
    ThreadPool(int num) :
        _stop(false) {
        for(int i = 0; i < num; ++i) {
            _threads.emplace_back([this] {
                while(!_stop) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> ul(_mtx);
                        _cv.wait(ul, [this] { return _stop || !_tasks.empty(); });

                        if(_stop) break;
                        task = _tasks.front();
                        _tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        stop();
        for(auto& t : _threads) {
            if(t.joinable()) {
                t.join();
            }
        }
    }

    void addTask(const Task& task) {
        {
            std::lock_guard<std::mutex> lg(_mtx);
            _tasks.push(task);
        }
        _cv.notify_one();
    }

private:
    void stop() {
        _stop = true;
        _cv.notify_all();
    }

private:
    std::atomic<bool> _stop;
    std::mutex _mtx;
    std::condition_variable _cv;
    std::vector<std::thread> _threads;
    std::queue<Task> _tasks;
};

#endif // _THREADPOOL_H

