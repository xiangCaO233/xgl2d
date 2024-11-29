#ifndef COREUSER_H
#define COREUSER_H

#include <condition_variable>
#include <mutex>
#include <thread>

class ParallelWorker {
    // 总线程数与完成工作的线程数
    int _threadcount;
    int _completedthreadcount;
    // 所有的线程
    std::thread *threads;
    // 线程同步变量
    std::mutex mtx;
    std::condition_variable cv;

    void *(*task)();

   public:
    // 构造ParallelWorker
    ParallelWorker(int threadcount);

    // 析构ParallelWorker
    virtual ~ParallelWorker();
};

#endif /* COREUSER_H */
