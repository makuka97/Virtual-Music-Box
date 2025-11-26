#include "music.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

void displayWelcome() {
    std::cout << "\n+====================================+\n";
    std::cout << "|      ** MUSIC BOX **               |\n";
    std::cout << "|      Terminal Edition              |\n";
    std::cout << "|      written by Micajah Nordyke    |\n";
    std::cout << "+====================================+\n\n";
}

void displayHelp() {
    std::cout << "  Commands:\n";
    std::cout << "  ------------------------------------\n";
    std::cout << "  play <file>  - Insert and play cylinder\n";
    std::cout << "  list         - Show available cylinders\n";
    std::cout << "  help         - Show this help\n";
    std::cout << "  quit         - Close the music box\n\n";
}

std::vector<std::string> listCylinders(const std::string& directory = ".") {
    std::vector<std::string> files;
    
    #ifdef _WIN32
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((directory + "\\*.txt").c_str(), &findData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string filename = findData.cFileName;
            // Skip template files
            if (filename[0] != '_') {
                files.push_back(filename);
            }
        } while (FindNextFile(hFind, &findData) != 0);
        FindClose(hFind);
    }
    #else
    DIR* dir = opendir(directory.c_str());
    if (dir != nullptr) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filename = entry->d_name;
            if (filename.length() > 4 && 
                filename.substr(filename.length() - 4) == ".txt" &&
                filename[0] != '_') {  // Skip template files
                files.push_back(filename);
            }
        }
        closedir(dir);
    }
    #endif
    
    std::sort(files.begin(), files.end());
    return files;
}

void waitForWinding() {
    std::cout << "\n  >> Wind the spring (press ENTER)...\n";
    std::cout << "  ";
    std::cin.get();
}

int main(int argc, char* argv[]) {
    MusicBox box;
    
    displayWelcome();
    displayHelp();
    
    while (true) {
        std::string input;
        
        std::cout << "  > ";
        std::getline(std::cin, input);
        
        // Trim whitespace
        size_t start = input.find_first_not_of(" \t");
        size_t end = input.find_last_not_of(" \t");
        if (start != std::string::npos) {
            input = input.substr(start, end - start + 1);
        }
        
        // Handle empty input
        if (input.empty()) {
            continue;
        }
        
        // Parse command
        if (input == "quit" || input == "exit" || input == "q") {
            std::cout << "\n  Closing the music box. Goodbye!\n\n";
            break;
        }
        else if (input == "help" || input == "h" || input == "?") {
            displayHelp();
        }
        else if (input == "list" || input == "ls") {
            std::cout << "\n  Available Cylinders:\n";
            std::cout << "  ------------------------------------\n";
            
            std::vector<std::string> cylinders = listCylinders();
            
            if (cylinders.empty()) {
                std::cout << "  No .txt song files found.\n";
                std::cout << "  Create a .txt file in this folder!\n\n";
            } else {
                for (const auto& file : cylinders) {
                    std::cout << "  * " << file << "\n";
                }
                std::cout << "\n  Play with: play <filename>\n\n";
            }
        }
        else if (input.substr(0, 5) == "play ") {
            std::string filename = input.substr(5);
            
            // Trim filename
            start = filename.find_first_not_of(" \t");
            if (start != std::string::npos) {
                filename = filename.substr(start);
            }
            
            if (filename.empty()) {
                std::cout << "\n  Error: No cylinder specified.\n";
                std::cout << "  Usage: play <filename>\n\n";
                continue;
            }
            
            // Visual loading effect
            std::cout << "\n  Inserting cylinder";
            for (int i = 0; i < 3; i++) {
                std::cout << "." << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
            std::cout << " OK\n";
            
            // Wait for user to "wind" the music box
            waitForWinding();
            
            // Play the cylinder
            box.play(filename);
            
            std::cout << "\n";
        }
        else {
            std::cout << "\n  Unknown command. Type 'help' for commands.\n\n";
        }
    }
    
    return 0;
}