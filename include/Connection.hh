#ifndef __CONNECTION_HH
#define __CONNECTION_HH

#include <mysql/mysql.h>
#include <string>
#include <ctime>
#include <chrono>

class Connection
{
public:
    Connection();
    ~Connection();
    
    bool connect(std::string ip, unsigned short port, std::string user, std::string password, std::string dbname);
    bool update(std::string sql);
    MYSQL_RES* query(std::string sql);

    void refreshAliveTime() { aliveTime_ = std::chrono::system_clock::now(); }
    double getAliveTime() { return std::chrono::duration<double>(std::chrono::system_clock::now() - aliveTime_).count() * 1000; }
private:
    MYSQL *conn_;
    std::chrono::time_point<std::chrono::system_clock> aliveTime_;
};

#endif
