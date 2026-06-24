#include <vector>
#include <optional>
#include <memory>
#include <iostream>

#include "collectors/ICollector.hpp"
#include "model/Metric.hpp"
#include "SystemMonitor.hpp"

/**
 * unique_ptr nao permite copias do objeto, ele tem apenas um dono, que neste caso é a instancia da classe SystemMonitor. A unica forma de passar o 
 * objeto para outro lado, é com o operador 'std::move' que passar o objeto para outro dono, e o ptr do dono anterior fica vazio, neste caso o novo 
 * dono passa a ser o vector collectors_. É bom porque assim sabe se exatamente quem lida com a alocaçao e libertaçao(?) de memoria deste objeto.
 */
void SystemMonitor::add_collector(std::unique_ptr<ICollector> collector) {
    collectors_.push_back(std::move(collector));
};

/**
 * auto é um tipo que deteta automaticamente o tipo da var, para nao ter de escriver sempre o tipo, por ser muito longo, ou só para nao estar sempre
 * a repeti lo. torna o codigo mais legivel.
 * Aqui nao estou a usar 'const' no 'auto&' porque o collector tem metodos nao const, que nao podem ser chamados com uma referencia const. 
 * const == nao muda estado.
 */
std::optional<std::vector<Metric>> SystemMonitor::collect_all() {
    std::vector<Metric> all_metrics;

    for (auto& collector : collectors_) {
        std::optional<std::vector<Metric>> metrics = collector->collect();
        if (!metrics.has_value())
        {
            std::cerr << "Error colleting metrics for " << collector->get_label() << "\n";
            return std::nullopt;
        }
        for (auto& metric : *metrics)
        {
            all_metrics.push_back(metric);
        }
    }

    return all_metrics;
};
