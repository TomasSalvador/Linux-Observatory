#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <format>

#include "model/Metric.hpp"
#include "collectors/NetworkCollector.hpp"

namespace {

    constexpr int DOWNLOAD_IDX = 0;
    constexpr int UPLOAD_IDX = 8;
    constexpr int INTERVAL_SECONDS = 2;

    struct NetworkInfo {
        unsigned long download;
        unsigned long upload;
    };

    std::optional<std::string> get_network_interface_line(const std::string& file_path, const std::string& interface) {
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            std::cerr << "Could not open the file in" << file_path << '\n';
            return std::nullopt;
        }
        std::string interface_label = interface + ':';
        std::string line;  
        while (std::getline(file, line)) {
            if (line.find(interface_label) == 0) { //checks if it's at the start of the line, where it should be
                return line;
            }
        }

        std::cerr << std::format("{} network interface not found in {}\n", interface, file_path);
        return std::nullopt;
    }

    std::optional<NetworkInfo> extract_network_info(const std::string& file_path, const std::string& interface) {
        std::optional<std::string> network_interface_line = get_network_interface_line(file_path, interface);

        if (!network_interface_line.has_value())
        {
            return std::nullopt;
        }

        std::istringstream network_info_stream(*network_interface_line);
        
        std::string label;
        network_info_stream >> label;

        NetworkInfo network_info = {0, 0};
        unsigned long temp = 0;
        int idx = 0;
        while (network_info_stream >> temp)
        {
            if (idx == DOWNLOAD_IDX)
            {
                network_info.download = temp;
            } else if (idx == UPLOAD_IDX) 
                {
                    network_info.upload = temp;
                    break;
                }
            idx++;
        }
        
        return network_info;
    }
}

NetworkCollector::NetworkCollector(const std::string& file_path, const std::string& interface) 
    : file_path_(file_path), interface_(interface) {}

std::optional<std::vector<Metric>> NetworkCollector::collect() {
    std::optional<NetworkInfo> network_info1 = extract_network_info(file_path_, interface_);
    if (!network_info1.has_value())
    {
        std::cerr << "First network info read failed\n";
        return std::nullopt;
    }

    sleep(INTERVAL_SECONDS);

    std::optional<NetworkInfo> network_info2 = extract_network_info(file_path_, interface_);
    if (!network_info2.has_value())
    {
        std::cerr << "Second network info read failed\n";
        return std::nullopt;
    }
    
    double download_delta_kb = static_cast<double>(network_info2->download - network_info1->download) / 1024;
    double upload_delta_kb = static_cast<double>(network_info2->upload - network_info1->upload) / 1024;

    std::vector<Metric> res = {{get_label(), "Download", download_delta_kb/INTERVAL_SECONDS, "KB/s"}, {get_label(), "Upload", upload_delta_kb/INTERVAL_SECONDS, "KB/s"}};

    return res;
}

std::string NetworkCollector::get_label() const {
    return "Network";
}
