#pragma once
#include <optional>
#include <string>
#include <vector>
#include <expected>

#include "model/Metric.hpp"
#include "ICollector.hpp"

class MemoryCollector : public ICollector {
public:
    explicit MemoryCollector(const std::string& file_path);
    std::expected<std::vector<Metric>, std::string> collect() override;
    std::string get_label() const override;

private:
    std::string file_path_;
};