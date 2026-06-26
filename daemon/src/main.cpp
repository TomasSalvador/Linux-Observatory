#include <iostream>
#include <string>
#include <unistd.h>
#include <format>
#include <optional>
#include <vector>
#include <memory>

#include "model/Metric.hpp"
#include "collectors/CpuCollector.hpp"
#include "collectors/MemoryCollector.hpp"
#include "collectors/NetworkCollector.hpp"
#include "SystemMonitor.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: sysobserverd <network_interface>\n";
        std::cerr << "Example: sysobserverd wlp2s0\n";
        return 1;
    }
    // nao é const std::string& porque o argv[1] é um char*, entao teria de criar uma std::string para criar a referencia, entao vai dar ao mesmo.
    std::string interface = argv[1];
    
    std::cout << "Starting System Monitoring App...\n\n";

    SystemMonitor system_monitor;

    system_monitor.add_collector(std::make_unique<MemoryCollector>(MemoryCollector("/proc/meminfo")));
    system_monitor.add_collector(std::make_unique<CpuCollector>(CpuCollector("/proc/stat")));
    system_monitor.add_collector(std::make_unique<NetworkCollector>(NetworkCollector("/proc/net/dev", interface)));

    while(true) {

        std::optional<std::vector<Metric>> all_metrics = system_monitor.collect_all();

        if (!all_metrics.has_value())
        {
            return 1;
        }

        for (const auto& metric : *all_metrics)
        {
            std::cout << std::format("{} {}: {:.2f}{}\n", metric.collector_label, metric.label, metric.value, metric.unit);    
        }

        std::cout << '\n';

        sleep(2);
    }
    
    return 0;
}