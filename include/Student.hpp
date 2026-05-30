#pragma once
#include <string>
#include <cstdint>

struct Student
{
    char account[10];
    std::string name;
    char phoneNumber[12];
    uint32_t age;
    char entryDate[8];
    bool isActive;
};