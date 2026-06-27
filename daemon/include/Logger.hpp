#pragma once
#include <vector>
#include <fstream>

#include "model/Metric.hpp"

class Logger {
public:
    explicit Logger(const std::string& log_file_path);
    void log(const std::vector<Metric>& metrics);
    void log_error(const std::string& error_message);
    bool is_open() const;
    ~Logger();

private:
    std::ofstream log_file_;
};