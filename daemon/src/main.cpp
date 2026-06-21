#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "memory_monitor.hpp"
#include "cpu_monitor.hpp"

int main()
{
    std::cout << "Starting Memory Monitoring App...\n";

    std::string mem_file_name("/proc/meminfo");
    std::string cpu_file_name("/proc/stat");

    while(true) {
        std::string mem_output = memory_monitor(mem_file_name);
        std::string cpu_output = cpu_monitor(cpu_file_name);

        std::cout << mem_output;
        std::cout << cpu_output;
        
        std::cout << "\n";

        sleep(2);
    }


    
    return 0;
}