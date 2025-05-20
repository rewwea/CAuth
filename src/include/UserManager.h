class UserManager {
    vector<shared_ptr<User>> users;
    DatabaseManager& db;

public:
    UserManager(DatabaseManager& db) : db(db) {
        users = db.loadUsers();
    }

    bool registerUser(const string& login, const string& plainPassword, const string& role) {
        shared_ptr<User> user;

        if (role == "Admin")
            user = make_shared<Admin>(login, plainPassword);
        else if (role == "User")
            user = make_shared<RegularUser>(login, plainPassword);
        else
            return false;

        if (!db.saveUser(*user))
            return false;

        users.push_back(user);
        return true;
    }

    bool auth(const string& login, const string& password) const {
        for (const auto& user : users) {
            if (user->getLogin() == login && user->auth(password))
                return true;
        }
        return false;
    }

    bool hasAccess(const string& login, const string& resource) const {
        for (const auto& user : users) {
            if (user->getLogin() == login)
                return user->hasAccess(resource);
        }
        return false;
    }
};