#pragma once
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class Logger {
public:
    static void log(const string& message) {
        time_t now = time(nullptr);
        cout << "[" << ctime(&now);
        cout << "] " << message << endl;
    }
};