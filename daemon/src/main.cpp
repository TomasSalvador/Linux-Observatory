#include <iostream>
#include <string>
#include <unistd.h>
#include <format>
#include <optional>

#include "model/SystemStats.hpp"
#include "collectors/CpuCollector.hpp"
#include "collectors/MemoryCollector.hpp"

int main()
{
    std::cout << "Starting System Monitoring App...\n\n";

    MemoryCollector mem_collector = MemoryCollector("/proc/meminfo");
    CpuCollector cpu_collector = CpuCollector("/proc/stat");

    while(true) {
        std::optional<double> mem_usage_perc = mem_collector.get_usage_percentage();
        if (!mem_usage_perc.has_value())
        {
            std::cerr << "Error getting memory usage stats\n";
            return 1;
        }
        
        std::optional<double> cpu_usage_perc = cpu_collector.get_usage_percentage();
        if (!cpu_usage_perc.has_value())
        {
            std::cerr << "Error getting cpu usage stats\n";
            return 1;
        }

        std::cout << std::format("Memory Usage: {:.2f}%\n", *mem_usage_perc);
        std::cout << std::format("CPU Usage: {:.2f}%\n", *cpu_usage_perc);
        std::cout << "\n";

        sleep(2);
    }
    
    return 0;
}