#include "UserManager.h"
#include <fstream>
#include <iostream>
UserManager::UserManager() : filename("data/users.txt") {}
void UserManager::registerUser(const std::string& username, const std::string& password, const std::string& role) {
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        std::cerr << "Unable to open user file" << std::endl;
        return;
    }
    file << username << " " << password << " " << role << std::endl;
    std::cout << "Registration successful!" << std::endl;
}
std::string UserManager::loginUser(const std::string& username, const std::string& password) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Unable to open user file" << std::endl;
        return "";
    }
    std::string storedUser, storedPass, role;
    while (file >> storedUser >> storedPass >> role) {
        if (storedUser == username) {
            if (storedPass == password) {
                std::cout << "Login successful!" << std::endl;
                return role;
            } else {
                std::cout << "Incorrect password!" << std::endl;
                return "";
            }
        }
    }
    std::cout << "User does not exist!" << std::endl;
    return "";
}
