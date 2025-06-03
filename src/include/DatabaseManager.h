#pragma once
#include <sqlite3.h>
#include <iostream>
#include <string>
#include "User.h"

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
        const char* sql =   "CREATE TABLE IF NOT EXISTS users ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            "username TEXT UNIQUE NOT NULL,"
                            "password_hash TEXT NOT NULL,"
                            "role TEXT NOT NULL,"
                            "salt TEXT NOT NULL);";

        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
            cerr << "Ошибка при создании таблицы: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }
    bool saveUser(const User& user) {
        const char* sql = "INSERT INTO users (username, password_hash, role, salt) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;
    
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
            return false;
        }
    
        sqlite3_bind_text(stmt, 1, user.getLogin().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, user.getPasswordHash().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, user.getRole().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, user.getSalt().c_str(), -1, SQLITE_STATIC);
    
        bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
        sqlite3_finalize(stmt);
        return ok;
    }
    
    vector<shared_ptr<User>> loadUsers() {
        const char* sql = "SELECT username, password_hash, role, salt FROM users;";
        sqlite3_stmt* stmt;
        vector<shared_ptr<User>> result;
    
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Prepare failed: " << sqlite3_errmsg(db) << endl;
            return result;
        }
    
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string login = (const char*)sqlite3_column_text(stmt, 0);
            string hash = (const char*)sqlite3_column_text(stmt, 1);
            string role = (const char*)sqlite3_column_text(stmt, 2);
            string salt = (const char*)sqlite3_column_text(stmt, 3);
    
            if (role == "Admin")
                result.push_back(make_shared<Admin>(login, hash, role, salt));
            else if (role == "User")
                result.push_back(make_shared<RegularUser>(login, hash, role, salt));
            else
                result.push_back(make_shared<User>(login, hash, role, salt)); // для совместимости
        }
    
        sqlite3_finalize(stmt);
        return result;
    }
};