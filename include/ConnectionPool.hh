#ifndef __CONNECTION_POOL_HH
#define __CONNECTION_POOL_HH

#include "Connection.hh"

#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>

class ConnectionPool
{
public:
    static ConnectionPool* instance();
    std::shared_ptr<Connection> consumeConnection();
    bool readParam(const std::string &path);
private:
    ConnectionPool();
    ~ConnectionPool();
    void produceConnectionTask();
    void scannerConnectionTask();

    std::string ip_;
    unsigned short port_;
    std::string username_;
    std::string password_;
    std::string dbname_;
    int initSize_;
    int maxSize_;
    int maxIdleTime_;
    int connectionTimeout_;

    std::queue<Connection*> connectionQueue_;
    std::mutex connectionQueueMutex_;
    std::atomic<int> connectionCount_;
    std::condition_variable cond_;

    std::atomic<bool> showdown_;
};

#endif
