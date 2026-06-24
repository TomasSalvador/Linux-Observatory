#pragma once
#include <vector>
#include <optional>
#include <memory>

#include "collectors/ICollector.hpp"
#include "model/Metric.hpp"

class SystemMonitor {
public:
    void add_collector(std::unique_ptr<ICollector> collector);
    std::optional<std::vector<Metric>> collect_all();

private:
    std::vector<std::unique_ptr<ICollector>> collectors_;
};