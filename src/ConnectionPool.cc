#include "ConnectionPool.hh"
#include "Logger.hh"

#include <fstream>
#include <sstream>
#include <thread>
#include <functional>
#include <chrono>

ConnectionPool::ConnectionPool()
{
    if (!readParam("config/param.cnf"))
        return;
    
    for (int i = 0; i < initSize_; ++i)
    {
        Connection *p = new Connection;
        p->connect(ip_, port_, username_, password_, dbname_);
        p->refreshAliveTime();
        connectionQueue_.push(p);
        ++connectionCount_;
    }

    std::thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
    produce.detach();
    std::thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
    scanner.detach();
}

ConnectionPool::~ConnectionPool()
{
}

ConnectionPool* ConnectionPool::instance()
{
    static ConnectionPool pool;
    return &pool;
}

bool ConnectionPool::readParam(const std::string &path)
{
    std::ifstream fs(path);
    if (!fs.is_open())
    {
        LOG(path + " file is not exist!");
        return false;
    }

    std::string line;
    while (std::getline(fs, line))
    {
        std::stringstream ss(line);
        std::string key;
        ss >> key;
        char semicolon;
        ss >> semicolon;

        if (key == "ip")
        {
            ss >> ip_;
        }
        else if (key == "port")
        {
            ss >> port_;
        }
        else if (key == "username")
        {
            ss >> username_;
        }
        else if (key == "password")
        {
            ss >> password_;
        }
        else if (key == "dbname")
        {
            ss >> dbname_;
        }
        else if (key == "initSize")
        {
            ss >> initSize_;
        }
        else if (key == "maxSize")
        {
            ss >> maxSize_;
        }
        else if (key == "maxIdleTime")
        {
            ss >> maxIdleTime_;
        }
        else if (key == "connectionTimeout")
        {
            ss >> connectionTimeout_;
        }
    }
    
    return true;
}

std::shared_ptr<Connection> ConnectionPool::consumeConnection()
{
    std::unique_lock<std::mutex> locker(connectionQueueMutex_);
    while (connectionQueue_.empty())
    {
        if (cond_.wait_for(locker, std::chrono::milliseconds(connectionTimeout_)) == std::cv_status::timeout)
        {
            if (connectionQueue_.empty())
            {
                LOG("get connection timeout...");
                return nullptr;
            }
        }
    }

    std::shared_ptr<Connection> sp(connectionQueue_.front(), [&](Connection *conn){
        std::unique_lock<std::mutex> locker(connectionQueueMutex_);
        conn->refreshAliveTime();
        connectionQueue_.push(conn);
    });
    connectionQueue_.pop();
    cond_.notify_all();

    return sp;    
}

void ConnectionPool::produceConnectionTask()
{
    for (;;)
    {
        std::unique_lock<std::mutex> locker(connectionQueueMutex_);
        while (!connectionQueue_.empty())
        {
            cond_.wait(locker);
        }

        if (connectionCount_ < maxSize_)
        {
            Connection *p = new Connection;
            p->connect(ip_, port_, username_, password_, dbname_);
            p->refreshAliveTime();
            connectionQueue_.push(p);
            ++connectionCount_;
        }
        cond_.notify_all();
    }
}

void ConnectionPool::scannerConnectionTask()
{
    for (;;)
    {
        std::this_thread::sleep_for(std::chrono::seconds(maxIdleTime_));
        std::unique_lock<std::mutex> locker(connectionQueueMutex_);
        while (connectionCount_ > initSize_)
        {
            Connection *p = connectionQueue_.front();

            if (p->getAliveTime() < maxIdleTime_ * 1000)
                break;

            connectionQueue_.pop();
            --connectionCount_;
            delete p;
        }
    }
}
