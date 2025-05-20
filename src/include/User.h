#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <random>

using namespace std;

class User {
protected:
    string login;
    string passwordHash;
    string role;
    string salt;

    static string generateSalt(size_t length = 16) {
        static const char chars[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dist(0, sizeof(chars) - 2);

        string s;
        for (size_t i = 0; i < length; ++i)
            s += chars[dist(gen)];
        return s;
    }

    static string sha256(const string& input) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)input.c_str(), input.size(), hash);

        stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
            ss << hex << setw(2) << setfill('0') << (int)hash[i];

        return ss.str();
    }

public:
    User(const string& login, const string& passwordPlain, const string& role)
        : login(login), role(role) {
        salt = generateSalt();
        passwordHash = sha256(passwordPlain + salt);
    }

    User(const string& login, const string& passwordHash, const string& role, const string& salt)
        : login(login), passwordHash(passwordHash), role(role), salt(salt) {}

    virtual ~User() = default;

    string getLogin() const { return login; }
    string getRole() const { return role; }
    string getPasswordHash() const { return passwordHash; }
    string getSalt() const { return salt; }

    virtual bool hasAccess(const string& resource) const {
        return false;
    }

    bool auth(const string& plainPassword) const {
        return sha256(plainPassword + salt) == passwordHash;
    }
};