#ifndef __CONNECTION_POOL_H__
#define __CONNECTION_POOL_H__
#include<iostream>
#include<queue>
#include<string>
#include<mutex>
#include<thread>
#include<memory>
#include<condition_variable>
#include "MySQLConn.h"

namespace cnnpool {

    // 连接池类
    class ConnectionPool {
    public:
        // 获取实例
        static ConnectionPool* getConnectionPool();
        // 禁止自己创建实例
        ConnectionPool(const ConnectionPool&) = delete;
        ConnectionPool& operator=(const ConnectionPool&) = delete;
        // 获取一个MySQL连接
        MySQLConn::ptr getConnection(); 
        // 析构函数，释放资源
        ~ConnectionPool();
    private:
        // 构造函数
        ConnectionPool();
        // 解析配置文件
        bool parseConfig();
        // 生产者线程调用
        void produceConnection();
        // 循环线程调用
        void recycleConnection();
        // 添加连接
        void addConnection();
    private:
        // 连接池 
        std::queue<MySQLConn*> m_connections;
        // 服务器ip地址
        std::string m_ip;
        // 用户名
        std::string m_user;
        // 密码
        std::string m_passwd;
        // 数据库名称
        std::string m_dbName;
        // 端口
        uint16_t m_port;
        // 连接池上限
        int m_maxSize;
        // 连接池下限
        int m_minSize;
        // 超时时长
        int m_timeout;
        // 最大空闲时长
        int m_maxIdleTime; 
        // 互斥锁
        std::mutex m_mutex;
        // 条件变量
        std::condition_variable m_cond;
    };
}

#endif