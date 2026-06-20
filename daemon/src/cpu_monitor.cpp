#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <format>
#include <unistd.h>

#include "cpu_monitor.hpp"

constexpr int idle_idx = 3;
constexpr int iowait_idx = 4;

struct CPU_Info {
    unsigned int total;
    unsigned int idle_iowait;
};

CPU_Info get_cpu_info(std::string cpu_info) {
    std::istringstream cpu_info_stream(cpu_info);

    std::string label;
    cpu_info_stream >> label;

    unsigned long total = 0;
    unsigned long idle_iowait = 0;
    unsigned long value = 0;
    
    unsigned short idx = 0;
    while (cpu_info_stream >> value) {
        total += value;
        if (idx == idle_idx || idx == iowait_idx)
        {
            idle_iowait += value;
        }
        idx++;
    }

    CPU_Info res = {total, idle_iowait};

    return res;
}

CPU_Info extract_cpu_usage_info(const std::string& file_name) {
    std::ifstream file(file_name);

    if (!file.is_open()) {
        std::cerr << "Could not open the file in" << file_name << '\n';
        return {};
    }

    std::string line;    
    std::getline(file, line);
    CPU_Info cpu_usage_percentage = get_cpu_info(line);

    file.close();

    return cpu_usage_percentage;
}


std::string cpu_monitor(const std::string& file_name) {

    CPU_Info cpu_info1 = extract_cpu_usage_info(file_name);

    sleep(3);

    CPU_Info cpu_info2 = extract_cpu_usage_info(file_name);

    unsigned long total_delta = cpu_info2.total - cpu_info1.total;
    unsigned long idle_iowait_delta = cpu_info2.idle_iowait - cpu_info1.idle_iowait;

    std::string res;

    if (total_delta != 0) {
        double cpu_usage_percentage = (1 - static_cast<double>(idle_iowait_delta) / static_cast<double>(total_delta)) * 100.0;
        res = std::format("CPU Usage: {:.2f}%\n", cpu_usage_percentage);
    } else {
        res = "Error reading CPU Usage\n";
    }

    return res;
    
}