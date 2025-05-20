#pragma once
#include <string>

using namespace std;

class User {
protected:
    string login;
    string passwordHash;
    string role;

public:
    User(const string& login, const string& passwordHash, const string& role)
        : login(login), passwordHash(passwordHash), role(role) {}

    virtual ~User() = default;

    string getLogin() const { return login; }
    string getRole() const { return role; }
    string getPasswordHash() const { return passwordHash; }

    virtual bool hasAccess(const string& resource) const {
        return false;
    }
};

class Admin : public User {
public:
    Admin(const string& login, const string& passwordHash)
        : User(login, passwordHash, "Admin") {}

    bool hasAccess(const string& resource) const override {
        return true;
    }
};

class RegularUser : public User {
public:
    RegularUser(const string& login, const string& passwordHash)
        : User(login, passwordHash, "User") {}

    bool hasAccess(const string& resource) const override {
        return resource == "home" || resource == "profile";
    }
};

class Guest : public User {
public:
    Guest() : User("guest", "", "Guest") {}

    bool hasAccess(const string& resource) const override {
        return resource == "home";
    }
};