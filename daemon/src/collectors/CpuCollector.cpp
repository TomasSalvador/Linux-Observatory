#include <optional>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <vector>

#include "collectors/CpuCollector.hpp"
#include "model/Metric.hpp"

/**
 * namespace sem nome, é tipo o 'static' no C ou, nao me lembro do nome, mas no Java, classes/metodos definidos dentro de outros classes/metodos.
 * É para nao serem visiveis para o exterior, só no ficheiro onde estao.
 */
namespace {
    constexpr int IDLE_IDX = 3;
    constexpr int IOWAIT_IDX = 4;

    struct CpuInfo {
        unsigned long total;
        unsigned long idle_iowait;
    };

    std::optional<std::string> get_cpu_info_from_file(const std::string& file_path) {
        // fecha a stream automaticamente quando a var que o referencia está fora do scope de uso (quando a funçao retorna).
        std::ifstream file(file_path);

        if (!file.is_open()) {
            std::cerr << "Could not open the file in" << file_path << '\n';
            return std::nullopt;
        }

        std::string line;    
        std::getline(file, line);
        
        return line;
    }

    std::optional<CpuInfo> extract_cpu_info(const std::string& file_path) {
        std::optional<std::string> cpu_info_string = get_cpu_info_from_file(file_path);
        if (!cpu_info_string.has_value())
        {
            return std::nullopt;
        }
        
        std::istringstream cpu_info_stream(*cpu_info_string);

        std::string label;
        cpu_info_stream >> label;

        unsigned long total = 0;
        unsigned long idle_iowait = 0;
        unsigned long value = 0;
        
        int idx = 0;
        while (cpu_info_stream >> value) {
            total += value;
            if (idx == IDLE_IDX || idx == IOWAIT_IDX)
            {
                idle_iowait += value;
            }
            idx++;
        }

        CpuInfo res = {total, idle_iowait};

        return res;
    }
}

/**
 * Para inicializar campos no construtor, usar o ' : x' antes de '{}' para ficaram logo com o valor, se só for feito dentro do '{}' elas primeiro
 * sao inicializados com um valor default(depende do seu construtor) e só depois é que faz set ao valor que se quer.
 */
CpuCollector::CpuCollector(const std::string& file_path) : file_path_(file_path) {}


std::optional<std::vector<Metric>> CpuCollector::collect() {
    std::optional<CpuInfo> cpu_info1 = extract_cpu_info(file_path_);

    if (!cpu_info1.has_value())
    {
        std::cerr << "First cpu info read failed\n";
        return std::nullopt;
    }

    sleep(3);

    std::optional<CpuInfo> cpu_info2 = extract_cpu_info(file_path_);

    if (!cpu_info2.has_value())
    {
        std::cerr << "Second cpu info read failed\n";
        return std::nullopt;
    }

    unsigned long total_delta = cpu_info2->total - cpu_info1->total;
    unsigned long idle_iowait_delta = cpu_info2->idle_iowait - cpu_info1->idle_iowait;

    if (total_delta == 0)
    {
        std::cerr << "CPU total delta is zero, cannot calculate usage\n";
        return std::nullopt;
    }    

    double usage = (1 - static_cast<double>(idle_iowait_delta) / static_cast<double>(total_delta)) * 100.0;

    return std::vector<Metric>{{get_label(), "Usage", usage, "%"}}; 
}

std::string CpuCollector::get_label() const {
    return "CPU";
}
