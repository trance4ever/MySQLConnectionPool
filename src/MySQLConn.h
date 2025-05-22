#ifndef __MYSQL_CONN_H__
#define __MYSQL_CONN_H__
#include<iostream>
#include<string>
#include<stdint.h>
#include<mysql/mysql.h>

namespace cnnpool {

    class MySQLConn {
    public:
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
    private:
        // 释放结果集
        void freeResult();
    private:
        MYSQL* m_conn = nullptr; 
        MYSQL_RES* m_result = nullptr;
        MYSQL_ROW m_row = nullptr;
    }; 

}


#endif