#pragma once
#include <string>
#include <optional>

// const std::string& é a referencia constante para o objeto string, apenas permite leitura.
std::optional<double> get_cpu_usage_percentage(const std::string& file_name);