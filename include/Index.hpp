#pragma once
#include <string>
#include <cstdint>

struct Index
{
    char account[10];
    long long offset;
    uint32_t size;
    bool isActive;
};
