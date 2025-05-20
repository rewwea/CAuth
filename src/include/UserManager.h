#pragma once
#include <vector>
#include <memory>
#include "User.h"

using namespace std;

class UserManager {
    vector<shared_ptr<User>> users;

public:
    void addUser(shared_ptr<User> user) {
        users.push_back(user);
    }

    // TODO: пока что заглушка, просто проверяет логин и пароль как строки
    bool auth(const string& login, const string& passwordHash) const {
        for (const auto& user : users) {
            if (user->getLogin() == login && user->getPasswordHash() == passwordHash) {
                return true;
            }
        }
        return false;
    }

    shared_ptr<User> findUser(const string& login) const {
        for (const auto& user : users) {
            if (user->getLogin() == login)
                return user;
        }
        return nullptr;
    }
};