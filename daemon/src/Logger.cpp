#include <iostream>
#include <string>
#include <format>
#include <vector>
#include <fstream>
#include <ctime>

#include "Logger.hpp"
#include "model/Metric.hpp"

using std::string;
using std::vector;

namespace {
    string get_current_timestamp() {
        std::time_t current_timestamp;
        time(&current_timestamp);
        std::tm* local_time = std::localtime(&current_timestamp);

        char time_buffer[20];
        std::strftime(time_buffer, sizeof(time_buffer), "%Y/%m/%d %H:%M:%S", local_time);

        return time_buffer;
    }
}

Logger::Logger(const string& log_file_path) 
    : log_file_(log_file_path, std::ios::app) {
    if (log_file_.is_open())
    {
        log_file_ << get_current_timestamp() + " | Starting System Observer Daemon\n";
        log_file_.flush();
    } else {
        std::cerr << "Logger failed to open:" + log_file_path + '\n';
    }   
}

Logger::~Logger() {
    if (log_file_.is_open())
    {
        log_file_ << get_current_timestamp() + " | Shutting down System Observer Daemon\n";
        log_file_.close();
    }
}

void Logger::log(const vector<Metric>& metrics) {
    string full_metrics_log = get_current_timestamp() + ';';
    for (const auto& metric : metrics)
    {
        full_metrics_log += std::format("{} {}: {:.2f} {};", metric.collector_label, metric.label, metric.value, metric.unit);
    }
    
    full_metrics_log += '\n';
    log_file_ << full_metrics_log;
    log_file_.flush();
}

/**
 * Adds new_line at the end of the error message.
 */
void Logger::log_error(const string& error_message) {
    log_file_ << get_current_timestamp() + " | Error with message: " + error_message + '\n' ;
    log_file_.flush();
}

bool Logger::is_open() const {
    return log_file_.is_open();
}