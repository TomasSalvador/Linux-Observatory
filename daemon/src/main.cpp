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
        std::vector<std::string> mem_output = memory_monitor(mem_file_name);
        std::string cpu_output = cpu_monitor(cpu_file_name);
        
        int size = mem_output.size();

        for (size_t i = 0; i < size; i++)
        {
            std::cout << mem_output[i];
        }
        std::cout << cpu_output;
        
        std::cout << "----------------------------------------\n";

        sleep(5);
    }


    
    return 0;
}