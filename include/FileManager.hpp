#pragma once
#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include "Student.hpp"

class FileManager
{
private:
    std::fstream in_out;

public:
    FileManager();

    std::vector<char> serializeStudent(const Student &st);
    Student deserializeStudent(std::vector<char> &buf);

    ~FileManager();
};
