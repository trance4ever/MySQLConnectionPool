#include "ConnectionPool.h"
#include <json/json.h>
#include<fstream>

namespace cnnpool { 
    
    ConnectionPool* ConnectionPool::getConnectionPool() {
        static ConnectionPool instance;
        return &instance;
    }

    bool ConnectionPool::parseConfig() {
        std::ifstream ifs("dbconfig.json"); 
        Json::Reader rd;
        Json::Value root;
        rd.parse(ifs, root);
        if(root.isObject()) {
            m_ip = root["ip"].asString();
            m_user = root["user"].asString();
            m_passwd = root["passwd"].asString();
            m_dbName = root["dbName"].asString();
            m_port = root["port"].asInt();
            m_maxSize = root["maxSize"].asInt();
            m_minSize = root["minSize"].asInt();
            m_timeout = root["timeout"].asInt();
            m_maxIdleTime = root["maxIdleTime"].asInt();
            return true;
        }
        return false;
    }

    ConnectionPool::ConnectionPool() {
        // 加载配置文件
        if(!parseConfig()) {
            return;
        }
        for(int i = 0; i < m_minSize; ++i) {
            addConnection();
        }
        std::thread producer(&ConnectionPool::produceConnection, this);
        std::thread recycler(&ConnectionPool::recycleConnection, this);
        producer.detach();
        recycler.detach();
    }

    void ConnectionPool::addConnection() {
        MySQLConn* conn = new MySQLConn;
        conn->connect(m_user, m_passwd, m_dbName, m_ip, m_port);
        conn->refreshAliveTime();
        m_connections.push(conn);
    }

    void ConnectionPool::produceConnection() {
        while(true) {
            std::unique_lock<std::mutex> lock(m_mutex);
            while(m_connections.size() >= m_minSize) {
                m_cond.wait(lock);
            }
            addConnection();
            m_cond.notify_all();
        }
    }
    
    void ConnectionPool::recycleConnection() {
       while(true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::lock_guard<std::mutex> lock(m_mutex);
            while(m_connections.size() > m_minSize) {
                MySQLConn* conn = m_connections.front();
                if(conn->getAliveTime() >= m_maxIdleTime) {
                    m_connections.pop();
                    delete conn;
                }
                else {
                    break;
                }
            }
       }
    }

    MySQLConn::ptr ConnectionPool::getConnection() {
        std::unique_lock<std::mutex> lock(m_mutex);
        while(m_connections.empty()) {
            if(std::cv_status::timeout == m_cond.wait_for(lock, std::chrono::milliseconds(m_timeout))) {
                if(m_connections.empty()) {
                    continue;
                }
            }
        }
        MySQLConn::ptr p(m_connections.front(), [this](MySQLConn* conn) {
            std::lock_guard<std::mutex> lock(m_mutex);
            conn->refreshAliveTime();
            m_connections.push(conn);
        });
        m_connections.pop();
        m_cond.notify_all();
        return p;
    }

    ConnectionPool::~ConnectionPool() {
        while(m_connections.size()) {
            MySQLConn* conn = m_connections.front();
            m_connections.pop();
            delete conn;
        }
    }
}