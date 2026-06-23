#pragma once
#include <optional>
#include <string>

#include "ICollector.hpp"

class MemoryCollector : public ICollector {
public:
    explicit MemoryCollector(const std::string& file_path);
    std::optional<double> get_usage_percentage() override;
    std::string get_label() const override;

private:
    std::string file_path_;
};