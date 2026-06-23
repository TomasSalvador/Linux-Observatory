#pragma once
#include <optional>
#include <string>

/**
 * Os metodos retornam 0 para indicar que nao têm implementaçao, e que quem implementa esta classe(interface/classe abstrata) tem de ter uma 
 * implementaçao para aquele(s) metodos.
 * O 'virtual ~IMonitor() = default;' é o destructor da classe, mas como isto é uma interface, tem o virtual para que se a classe que implementa
 * esta interface tiver o seu destructor, usa esse em vez do da interface. O default é para ter o comportamento 'default', que neste caso é nao 
 * fazer nada, que está certo porque isto é uma interface. O'~' indica que aquele metodo é o destructor.
 * O 'const' no metodo get_label() indica que este metodo nao altera o estado o objeto. Quando se tem uma referencia constante do objeto (const 
 * ICollector&) os unicos metodos que se podem usar sao os marcados com 'const'.
 */
class ICollector {
public:
    virtual std::optional<double> get_usage_percentage() = 0;
    virtual std::string get_label() const = 0;
    virtual ~ICollector() = default;
};