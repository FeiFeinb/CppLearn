#include "AllHeader.h"

#include "CppUtilities.h"

int main()
{
    char data = std::plus<>{}(10, 'A');
    std::cout << data;

    // auto nums = generate_random_container<std::vector>(100, 0, 100);
    // for (auto& data : nums)
    //     std::cout << data << " ";
}