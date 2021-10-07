//
// Created by 12784 on 2021/10/7.
//

#ifndef CPPLEARN_CLASSESFORTEST_H
#define CPPLEARN_CLASSESFORTEST_H

#include "AllHeader.h"

struct Magic_Int
{
    int data;
    ~Magic_Int();
};

struct POD_Int
{
    int data;
};

struct POD_Str
{
    std::string name;
};

struct NonCopyStruct_Str
{
private:
    std::string name;
public:
    NonCopyStruct_Str() = default;
    NonCopyStruct_Str(std::string);
    NonCopyStruct_Str(const NonCopyStruct_Str&) = delete;
    NonCopyStruct_Str(NonCopyStruct_Str&&) noexcept;

    NonCopyStruct_Str& operator=(const NonCopyStruct_Str&) = delete;
    NonCopyStruct_Str& operator=(NonCopyStruct_Str&&) noexcept;
};

struct StudentInfo_Str
{
private:
    std::string name;
public:
    // 初始化用
    StudentInfo_Str(std::string);
    // 输出用
    friend std::ostream& operator<<(std::ostream&, const StudentInfo_Str&);
};

struct NormalStruct_Int
{
    int data;
    NormalStruct_Int();
    NormalStruct_Int(int);
    ~NormalStruct_Int();
};

struct NormalStruct_Str
{
    std::string name;
    NormalStruct_Str();
    NormalStruct_Str(std::string);
    ~NormalStruct_Str();
};

struct NormalStructDerived_Int : public NormalStruct_Int
{
    int derivedData;
    NormalStructDerived_Int();
    NormalStructDerived_Int(int _baseData, int _derivedData);
    ~NormalStructDerived_Int();
};

#endif //CPPLEARN_CLASSESFORTEST_H
