#include "DatabaseManager.h"
#include "UserManager.h"
#include "NetworkManager.h"

int main() {
    DatabaseManager db("auth.db");
    UserManager manager(db);
    NetworkManager network(manager);
    network.start();
    return 0;
}