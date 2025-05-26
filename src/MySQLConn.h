#ifndef __MYSQL_CONN_H__
#define __MYSQL_CONN_H__
#include<iostream>
#include<string>
#include<chrono>
#include<memory>
#include<stdint.h>
#include<mysql/mysql.h>

namespace cnnpool {

    class MySQLConn {
    public:
        typedef std::shared_ptr<MySQLConn> ptr;
        // 构造函数
        MySQLConn();
        // 析构函数
        ~MySQLConn();
        // 连接数据库
        bool connect(std::string user, std::string passwd, std::string dbname, std::string ip, uint16_t port = 3306);
        // 更新数据库
        bool update(std::string sql);
        // 查询数据库 
        bool query(std::string sql);
        // 遍历查询得到的结果集
        bool next();
        // 获取记录中的字段值
        std::string value(int index);
        // 事务操作
        bool transaction();
        // 提交事务
        bool commit();
        // 回滚事务
        bool rollback();
        // 刷新空闲时间点
        void refreshAliveTime(); 
        // 计算连接存活的总时长
        long long getAliveTime();
    private:
        // 释放结果集
        void freeResult();
    private:
        MYSQL* m_conn = nullptr; 
        MYSQL_RES* m_result = nullptr;
        MYSQL_ROW m_row = nullptr;
        // 绝对时钟
        std::chrono::steady_clock::time_point m_aliveTime;
    }; 

}


#endif