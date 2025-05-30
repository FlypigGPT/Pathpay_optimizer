#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <string>
class UserManager {
private:
    std::string filename;
public:
    UserManager();
    void registerUser(const std::string& username, const std::string& password, const std::string& role);
    std::string loginUser(const std::string& username, const std::string& password);
};
#endif // USERMANAGER_H
