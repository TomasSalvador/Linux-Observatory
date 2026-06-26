#pragma once
#include <string>
#include <vector>

#include "ICollector.hpp"
#include "model/Metric.hpp"


class NetworkCollector : public ICollector {
public:
    explicit NetworkCollector(const std::string& file_path, const std::string& interface);
    std::optional<std::vector<Metric>> collect() override;
    std::string get_label() const override;

private:
    std::string file_path_;
    std::string interface_;
};
