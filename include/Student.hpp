#pragma once
#include <string>
#include <cstdint>


struct Student
{
    bool isActive = true;
    char account[10];
    std::string name;
    char phoneNumber[12];
    uint32_t age;
    char entryDate[8];
};