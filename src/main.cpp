#include <iostream>
#include "UserManager.h"
#include "DatabaseManager.h"

using namespace std;

int main() {
    cout << "Auth Server Started." << endl;

    DatabaseManager db("auth.db");
    UserManager manager;

    return 0;
}