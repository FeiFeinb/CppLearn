//
// Created by 12784 on 2021/10/7.
//

#include "ClassesForTest.h"

Magic_Int::~Magic_Int() {}

NonCopyStruct_Str::NonCopyStruct_Str(std::string _name) : name(std::move(_name)) {}

NonCopyStruct_Str::NonCopyStruct_Str(NonCopyStruct_Str&& _other) noexcept: name(std::move(_other.name)) {}

NonCopyStruct_Str& NonCopyStruct_Str::operator=(NonCopyStruct_Str&& _other) noexcept
{
    name = std::move(_other.name);
    return *this;
}

StudentInfo_Str::StudentInfo_Str(std::string _name) : name(std::move(_name)) {}

std::ostream& operator<<(std::ostream& os, const StudentInfo_Str& _info)
{
    return os << _info.name;
}

StudentInfo_Int::StudentInfo_Int(int _age) : age(_age) {}

std::ostream& operator<<(std::ostream& os, const StudentInfo_Int& _info)
{
    return os << _info.age;
}


NormalStruct_Int::NormalStruct_Int() : data(0)
{
    std::cout << "default create" << std::endl;
}

NormalStruct_Int::NormalStruct_Int(int _data) : data(_data)
{
    std::cout << "param create" << std::endl;
}

NormalStruct_Int::~NormalStruct_Int()
{
    std::cout << "destroy" << std::endl;
}


NormalStruct_Str::NormalStruct_Str() : name()
{
    std::cout << "default create" << std::endl;
}

NormalStruct_Str::NormalStruct_Str(std::string _name) : name(std::move(_name))
{
    std::cout << "param create" << std::endl;
}

NormalStruct_Str::~NormalStruct_Str()
{
    std::cout << "destroy" << std::endl;
}

NormalStructDerived_Int::NormalStructDerived_Int() : derivedData(0)
{
    std::cout << "derived default create" << std::endl;
}

NormalStructDerived_Int::NormalStructDerived_Int(int _baseData, int _derivedData) : NormalStruct_Int(_baseData), derivedData(_derivedData) {}

NormalStructDerived_Int::~NormalStructDerived_Int()
{
    std::cout << "derived destroy" << std::endl;
}