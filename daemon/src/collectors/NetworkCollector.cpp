#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <format>
#include <expected>

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

    std::expected<std::string, std::string> get_network_interface_line(const std::string& file_path, const std::string& interface) {
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            return std::unexpected("Could not open the file in" + file_path + '\n');
        }
        std::string interface_label = interface + ':';
        std::string line;  
        while (std::getline(file, line)) {
            if (line.find(interface_label) == 0) { //checks if it's at the start of the line, where it should be
                return line;
            }
        }

        return std::unexpected(std::format("{} network interface not found in {}\n", interface, file_path));
    }

    std::expected<NetworkInfo, std::string> extract_network_info(const std::string& file_path, const std::string& interface) {
        std::expected<std::string, std::string> network_interface_line = get_network_interface_line(file_path, interface);

        if (!network_interface_line.has_value())
        {
            return std::unexpected(network_interface_line.error());
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

std::expected<std::vector<Metric>, std::string> NetworkCollector::collect() {
    std::expected<NetworkInfo, std::string> network_info1 = extract_network_info(file_path_, interface_);
    if (!network_info1.has_value())
    {
        return std::unexpected("First network info read failed | " + network_info1.error());
    }

    sleep(INTERVAL_SECONDS);

    std::expected<NetworkInfo, std::string> network_info2 = extract_network_info(file_path_, interface_);
    if (!network_info2.has_value())
    {
        return std::unexpected("Second network info read failed | " + network_info2.error());
    }
    
    double download_delta_kb = static_cast<double>(network_info2->download - network_info1->download) / 1024;
    double upload_delta_kb = static_cast<double>(network_info2->upload - network_info1->upload) / 1024;

    std::vector<Metric> res = {{get_label(), "Download", download_delta_kb/INTERVAL_SECONDS, "KB/s"}, {get_label(), "Upload", upload_delta_kb/INTERVAL_SECONDS, "KB/s"}};

    return res;
}

std::string NetworkCollector::get_label() const {
    return "Network";
}
