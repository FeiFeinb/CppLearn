#include "Utilities/AllHeader.h"

template<typename T>
void print_vec(T& container)
{
    for (auto& i : container)
        std::cout << i << std::endl;
}

template<typename T>
void print_map(T& container)
{
    for (auto& p : container)
        std::cout << p.first << " " << p.second << std::endl;
}
