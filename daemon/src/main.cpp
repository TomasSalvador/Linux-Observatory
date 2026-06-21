#include <iostream>
#include <string>
#include <unistd.h>
#include <format>
#include <optional>

#include "memory_monitor.hpp"
#include "cpu_monitor.hpp"

int main()
{
    std::cout << "Starting System Monitoring App...\n";
    std::cout << "\n";

    std::string mem_file_name("/proc/meminfo");
    std::string cpu_file_name("/proc/stat");

    while(true) {
        std::optional<double> mem_usage_perc = get_memory_usage_percentage(mem_file_name);
        if (!mem_usage_perc.has_value())
        {
            std::cerr << "Error getting memory usage stats\n";
            return 1;
        }
        
        std::optional<double> cpu_usage_perc = get_cpu_usage_percentage(cpu_file_name);
        if (!cpu_usage_perc.has_value())
        {
            std::cerr << "Error getting cpu usage stats\n";
            return 1;
        }

        std::cout << std::format("Memory Usage: {:.2f}%\n", mem_usage_perc.value());
        std::cout << std::format("CPU Usage: {:.2f}%\n", cpu_usage_perc.value());
        
        std::cout << "\n";

        sleep(2);
    }
    
    return 0;
}