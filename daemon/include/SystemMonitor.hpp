#pragma once
#include <vector>
#include <optional>
#include <memory>

#include "collectors/ICollector.hpp"
#include "model/Metric.hpp"
#include "Logger.hpp"

class SystemMonitor {
public:
    explicit SystemMonitor(Logger& logger);
    void add_collector(std::unique_ptr<ICollector> collector);
    std::optional<std::vector<Metric>> collect_all();

private:
    std::vector<std::unique_ptr<ICollector>> collectors_;
    Logger& logger_;
};