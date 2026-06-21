#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <sstream>
#include <optional>

#include "memory_monitor.hpp"

/* 
std::string_view é apenas um ponteiro para onde o char de inicio da string, nao lida com a alocaçao da memoria, por isso há um risco de
eventualmente o valor no endereço para onde aponta mudar, pois nao controlo sobre isso. No caso de constantes, nao tem esse risco por o
seu valor vao strings literais, e estas têm o mesmo tempo de vida da aplicaçao.
*/
constexpr std::string_view MEM_TOTAL_LABEL = "MemTotal:";
constexpr std::string_view MEM_AVAILABLE_LABEL = "MemAvailable:";

double calculate_mem_usage_percentage(const std::string& mem_total_line, const std::string& mem_avail_line) {
    std::istringstream total_stream(mem_total_line);
    std::istringstream avail_stream(mem_avail_line);

    std::string label, unit;
    unsigned long total_mem = 0, avail_mem = 0;

    /*
     * O operador '<<' ou '>>' lê de um lado até encontrar um espaço, mudança de linha ou final de string e escreve no outro lado.
     * Dá para fazer a operaçao em sequencia, como o exemplo abaixo, faz o que disse com base na primeira string stream (total_stream/avail_stream).
     * Faz logo o cast para o tipo da var para onde está a escrever.
     */
    total_stream >> label >> total_mem >> unit;
    avail_stream >> label >> avail_mem >> unit;

    double used_mem = static_cast<double>(total_mem - avail_mem);

    return (used_mem / total_mem) * 100.0;
}

/* 
const std::string& é a referencia constante para o objeto string, apenas permite leitura, se passar std::string, cria uma copia.
cria uma copia qualquer que seja o tipo, por isso nem sempre é o mais otimizado, para tipos primitivos nao vai valer a pena, porque sao primitivos,
entao nao há problema de referencias - que nao é um problema no c++, mas tambem nao é preciso alocar memoria para guardar o valor - e porque a 
maior parte é menor do que um registo - ex. int sao 4B e um registo num sistema a 64b é 8B.
------
A rough rule of thumb (this comes from the C++ Core Guidelines): if a type is bigger than roughly two machine words and doesn't have trivial 
copy semantics, default to const T& for read-only parameters; otherwise pass by value.
*/
std::optional<double> get_memory_usage_percentage(const std::string& file_name) {

    std::ifstream file(file_name);

    if (!file.is_open()) {
        std::cerr << "Could not open the file in" << file_name << '\n';
        return std::nullopt;
    }

    std::string mem_total_line;
    std::string mem_avail_line;

    std::string line;    
    while (std::getline(file, line)) {
        if (line.find(MEM_TOTAL_LABEL) != std::string::npos) {
            mem_total_line = line + '\n';
        }
        else if (line.find(MEM_AVAILABLE_LABEL) != std::string::npos) {
            mem_avail_line = line + '\n';
        }
    }

    if (mem_avail_line.empty() || mem_total_line.empty())
    {
        std::cerr << "Could not find the required memory fields in " << file_name << "\n";
        return std::nullopt;
    }    

    return calculate_mem_usage_percentage(mem_total_line, mem_avail_line);
}