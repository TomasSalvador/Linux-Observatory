#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "memory_monitor.hpp"

int main()
{
    std::cout << "Starting Memory Monitoring App...\n";

    std::string mem_file_name("/proc/meminfo");

    while(true) {
        std::vector<std::string> mem_output = memory_monitor(mem_file_name);

        int size = mem_output.size();

        for (size_t i = 0; i < size; i++)
        {
            std::cout << mem_output[i];
        }
        std::cout << "----------------------------------------\n";

        sleep(5);
    }


    
    return 0;
}