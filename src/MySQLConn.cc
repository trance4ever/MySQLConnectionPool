#include "MySQLConn.h"

namespace cnnpool {

    MySQLConn::MySQLConn() {
        m_conn = mysql_init(nullptr);
        mysql_set_character_set(m_conn, "utf8");
    }

    MySQLConn::~MySQLConn() {
        if (m_conn != nullptr) {
            mysql_close(m_conn);
            m_conn = nullptr;
        }
        freeResult();
    }

    bool MySQLConn::connect(std::string user, std::string passwd, std::string dbname, std::string ip, uint16_t port) {
        MYSQL* ptr = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passwd.c_str(), dbname.c_str(), port, nullptr, 0);
        return ptr != nullptr;
    }

    bool MySQLConn::update(std::string sql) {
        if (mysql_query(m_conn, sql.c_str()) != 0) {
            return false;
        }
        return true;
    }

    bool MySQLConn::query(std::string sql) {
        freeResult();
        if (mysql_query(m_conn, sql.c_str()) != 0) {
            return false;
        }
        m_result = mysql_store_result(m_conn); 
        return true;
    }

    bool MySQLConn::next() {
        if(m_result != nullptr) {
            m_row = mysql_fetch_row(m_result);
            if(m_row != nullptr) {
                return true;
            }
        }
        return false;
    }

    std::string MySQLConn::value(int index) {
        int rowCnt = mysql_num_fields(m_result);
        if(index >= rowCnt || index < 0) {
            return std::string();
        } 
        char* val = m_row[index];
        unsigned long length = mysql_fetch_lengths(m_result)[index];
        return std::string(val, length);
    }

    bool MySQLConn::transaction() {
        return mysql_autocommit(m_conn, false);
    }

    bool MySQLConn::commit() {
        return mysql_commit(m_conn); 
    }

    bool MySQLConn::rollback() {
        return mysql_rollback(m_conn);
    }

    void MySQLConn::freeResult() {
        if(m_result) {
        mysql_free_result(m_result);
        m_result = nullptr;
        }
    }

    void MySQLConn::refreshAliveTime() {
        m_aliveTime = std::chrono::steady_clock::now();
    }

    long long MySQLConn::getAliveTime() {
        std::chrono::nanoseconds duration = std::chrono::steady_clock::now() - m_aliveTime;
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        return ms.count();
    }
}