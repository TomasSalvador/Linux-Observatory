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
#include "SystemMonitor.hpp"

int main()
{
    std::cout << "Starting System Monitoring App...\n\n";

    SystemMonitor system_monitor;

    system_monitor.add_collector(std::make_unique<MemoryCollector>(MemoryCollector("/proc/meminfo")));
    system_monitor.add_collector(std::make_unique<CpuCollector>(CpuCollector("/proc/stat")));

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