#pragma once
#include <optional>
#include <string>
#include <vector>
#include <optional>

#include "model/Metric.hpp"
#include "ICollector.hpp"

class MemoryCollector : public ICollector {
public:
    explicit MemoryCollector(const std::string& file_path);
    std::optional<std::vector<Metric>> collect() override;
    std::string get_label() const override;

private:
    std::string file_path_;
};