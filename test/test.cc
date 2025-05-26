#include<iostream>
#include<memory>
#include "../src/ConnectionPool.h"
using namespace std;
using namespace cnnpool;

void op1(int begin, int end) {
    for(int i = begin; i < end; ++i) {
        MySQLConn conn;
        conn.connect("trance", "12345678", "qsl", "127.0.0.1");
        char sql[1024] = {0};
        sprintf(sql, "insert into person values (%d, %d, 'nan', 'tom')", i, i + 20);
        conn.update(sql);
    }
}

void op2(ConnectionPool* pool, int begin, int end) {
    for(int i = begin; i < end; ++i) {
        MySQLConn::ptr conn = pool->getConnection();
        char sql[1024] = {0};
        sprintf(sql, "insert into person values (%d, %d, 'nan', 'tom')", i, i + 20);
        conn->update(sql);
    }
}

void test1() {

#if 0
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    op1(0, 500);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    auto length = end - begin;
    cout << "op1 time: " << length.count() / 1000000 << " ms" << endl;
# else
    ConnectionPool* pool = ConnectionPool::getConnectionPool();
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    op2(pool, 0, 500);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    auto length = end - begin;
    cout << "op1 time: " << length.count() / 1000000 << " ms" << endl;

#endif

}

int query() {
    MySQLConn conn;
    conn.connect("trance", "12345678", "qsl", "127.0.0.1");
    string sql = "insert into person values (5, 25, 'nan', 'tom')";
    bool flag = conn.update(sql);
    cout << "flag value: " << flag << endl;
    sql = "select * from person";
    flag = conn.query(sql);
    while(flag && conn.next()) {
        cout << conn.value(0) << ","
             << conn.value(1) << ","
             << conn.value(2) << ","
             << conn.value(3) << endl;
    }
    return 0;
}

int main() {
    test1();
    return 0;
}