#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

class Logger {
public:
    enum Level { INFO, WARNING, ERROR };

    static void log(const string& msg, Level lvl = INFO) {
        ofstream logFile("log.txt", ios::app);
        string prefix;

        switch (lvl) {
            case INFO:    prefix = "[INFO] "; break;
            case WARNING: prefix = "[WARN] "; break;
            case ERROR:   prefix = "[ERROR] "; break;
        }

        time_t now = time(nullptr);
        string timestamp = string(ctime(&now));
        timestamp.pop_back(); // убрать \n

        string fullMsg = prefix + timestamp + " - " + msg;

        cout << fullMsg << endl;
        logFile << fullMsg << endl;
        logFile.close();
    }
};