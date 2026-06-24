#pragma once
#include <string>

struct Metric {
    std::string collector_label;
    std::string label;
    double value;
    std::string unit;
};

