// logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

class Logger {
public:
    Logger(const std::string& filename) : log_file(filename, std::ios::app) {
        if (!log_file.is_open()) {
            std::cerr << "Error opening log file!" << std::endl;
        }
    }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(const std::string& message) {
        if (log_file.is_open()) {
            // Get current time
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            log_file << std::ctime(&now_c) << ": " << message << std::endl;
        }
    }

private:
    std::ofstream log_file;
};

#endif // LOGGER_H

