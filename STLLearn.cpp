#include "AllHeader.h"

class NonCopyClass
{
private:
    std::string name;
public:
    NonCopyClass() = default;

    NonCopyClass(std::string _name) : name(std::move(_name)) {}

    NonCopyClass(const NonCopyClass& _copy) = delete;

    NonCopyClass(NonCopyClass&& _copy) : name(std::move(_copy.name)) {}

    NonCopyClass& operator=(const NonCopyClass& _copy) = delete;

    NonCopyClass& operator=(NonCopyClass&& _copy) = delete;
};

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
struct StudentNum
{
    int number;
    // ��ʼ����
    StudentNum(int _number) : number(_number) {}
    // �����������
    StudentNum(const StudentNum&) = default;
    // �����
    friend std::ostream& operator<<(std::ostream& os, const StudentNum& studentNum) { return os << studentNum.number; }
};
