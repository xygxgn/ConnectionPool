#ifndef __CONNECTION_HH
#define __CONNECTION_HH

#include <mysql/mysql.h>
#include <string>
#include <ctime>

class Connection
{
public:
    Connection();
    ~Connection();
    
    bool connect(std::string ip, unsigned short port, std::string user, std::string password, std::string dbname);
    bool update(std::string sql);
    MYSQL_RES* query(std::string sql);

    void refreshAliveTime() { aliveTime_ = std::clock(); }
    std::clock_t getAliveTime() { return std::clock() - aliveTime_; }
private:
    MYSQL *conn_;
    std::clock_t aliveTime_;
};

#endif
