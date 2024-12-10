#include "ConnectionPool.hh"

#include <iostream>

#define ENABLE_THREADPOOL 1

int main()
{
	// char sql[1024] = {0};
	// sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
	// Connection conn;
	// conn.connect("127.0.0.1", 3306, "pulsar", "9420", "test");
    // conn.update(sql);

    std::chrono::time_point<std::chrono::system_clock> beginTime = std::chrono::system_clock::now();
#if ENABLE_THREADPOOL
    for (int i = 0; i < 1000; ++i)
    {
        ConnectionPool *pool = ConnectionPool::instance();
        char sql[1024] = {0};
        sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
        std::shared_ptr<Connection> sp = pool->consumeConnection();
        if (sp == nullptr)
        {
            std::cout << "get sql connection failed!" << std::endl;
            continue;
        }
        sp->update(sql);
    }
#else
    for (int i = 0; i < 1000; ++i)
    {
        char sql[1024] = {0};
        sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s')", "zhang san", 20, "male");
        Connection conn;
        conn.connect("127.0.0.1", 3306, "pulsar", "9420", "test");
        conn.update(sql);
    }
#endif
    std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
    std::cout << "timecosts: " << std::chrono::duration<double>(endTime - beginTime).count() * 1000 << "ms" << std::endl;

    return 0;
}