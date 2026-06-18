#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "cpu_monitor.hpp"


std::vector<std::string> cpu_monitor(const std::string& file_name) {

    std::ifstream file(file_name);

    if (!file.is_open()) {
        std::cerr << "Could not open the file in" << file_name << '\n';
        return {};
    }

    return {};
}