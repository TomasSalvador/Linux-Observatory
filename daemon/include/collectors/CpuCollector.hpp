#pragma once
#include <optional>
#include <string>

#include "ICollector.hpp"

class CpuCollector : public ICollector {
public:
    explicit CpuCollector(const std::string& file_path);
    std::optional<double> get_usage_percentage() override;
    std::string get_label() const override;

private:
    std::string file_path_;
};