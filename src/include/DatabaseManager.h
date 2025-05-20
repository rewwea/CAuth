#pragma once
#include <sqlite3.h>
#include <iostream>
#include <string>

using namespace std;

class DatabaseManager {
    sqlite3* db;

public:
    DatabaseManager(const string& dbPath) {
        if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
            cerr << "Ошибка при открытии базы данных: " << sqlite3_errmsg(db) << endl;
        } else {
            createTable();
        }
    }

    ~DatabaseManager() {
        sqlite3_close(db);
    }

    void createTable() {
        const char* sql = "CREATE TABLE IF NOT EXISTS users ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "username TEXT UNIQUE NOT NULL,"
                          "password_hash TEXT NOT NULL,"
                          "role TEXT NOT NULL);";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
            cerr << "Ошибка при создании таблицы: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }
};