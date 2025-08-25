#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <cctype>
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

// Function to hash password using SHA-256
std::string hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password.c_str(), password.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Function to validate username
bool isValidUsername(const std::string& username) {
    if (username.empty() || username.length() < 3 || username.length() > 20) {
        std::cout << "Username must be between 3 and 20 characters.\n";
        return false;
    }
    
    for (char c : username) {
        if (!std::isalnum(c) && c != '_') {
            std::cout << "Username can only contain letters, numbers, and underscores.\n";
            return false;
        }
    }
    return true;
}

// Function to validate password
bool isValidPassword(const std::string& password) {
    if (password.empty() || password.length() < 6) {
        std::cout << "Password must be at least 6 characters long.\n";
        return false;
    }
    
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (char c : password) {
        if (std::isupper(c)) hasUpper = true;
        if (std::islower(c)) hasLower = true;
        if (std::isdigit(c)) hasDigit = true;
    }
    
    if (!hasUpper || !hasLower || !hasDigit) {
        std::cout << "Password must contain at least one uppercase letter, one lowercase letter, and one digit.\n";
        return false;
    }
    return true;
}

// Function to check if username already exists
bool usernameExists(const std::string& username) {
    std::string filename = "users/" + username + ".dat";
    return fs::exists(filename);
}

// Function to register a new user
bool registerUser() {
    std::string username, password;
    
    std::cout << "\n=== REGISTRATION ===\n";
    std::cout << "Enter username: ";
    std::cin >> username;
    
    if (!isValidUsername(username)) {
        return false;
    }
    
    if (usernameExists(username)) {
        std::cout << "Username already exists. Please choose a different username.\n";
        return false;
    }
    
    std::cout << "Enter password: ";
    std::cin >> password;
    
    if (!isValidPassword(password)) {
        return false;
    }
    
    // Create users directory if it doesn't exist
    if (!fs::exists("users")) {
        fs::create_directory("users");
    }
    
    // Hash the password and store user data
    std::string hashedPassword = hashPassword(password);
    std::string filename = "users/" + username + ".dat";
    
    std::ofstream userFile(filename);
    if (userFile.is_open()) {
        userFile << hashedPassword;
        userFile.close();
        std::cout << "Registration successful! Welcome, " << username << "!\n";
        return true;
    } else {
        std::cout << "Error: Could not create user file.\n";
        return false;
    }
}

// Function to login user
bool loginUser() {
    std::string username, password;
    
    std::cout << "\n=== LOGIN ===\n";
    std::cout << "Enter username: ";
    std::cin >> username;
    
    if (!fs::exists("users/" + username + ".dat")) {
        std::cout << "Username not found. Please register first.\n";
        return false;
    }
    
    std::cout << "Enter password: ";
    std::cin >> password;
    
    // Read stored hash
    std::string filename = "users/" + username + ".dat";
    std::ifstream userFile(filename);
    std::string storedHash;
    
    if (userFile.is_open()) {
        userFile >> storedHash;
        userFile.close();
    } else {
        std::cout << "Error: Could not read user data.\n";
        return false;
    }
    
    // Hash input password and compare
    std::string inputHash = hashPassword(password);
    
    if (inputHash == storedHash) {
        std::cout << "Login successful! Welcome back, " << username << "!\n";
        return true;
    } else {
        std::cout << "Invalid password. Please try again.\n";
        return false;
    }
}

// Function to display main menu
void displayMenu() {
    std::cout << "\n=== LOGIN & REGISTRATION SYSTEM ===\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    int choice;
    
    std::cout << "Welcome to the Login and Registration System!\n";
    
    while (true) {
        displayMenu();
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                std::cout << "Thank you for using the system. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
        
        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    return 0;
}
