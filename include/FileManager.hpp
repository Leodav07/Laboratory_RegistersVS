#pragma once
#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include "Student.hpp"
#include "Index.hpp"

class FileManager
{
private:
    std::fstream in_out;

public:
    FileManager();

    std::vector<char> serializeStudent(const Student &st);
    Student deserializeStudent(const std::vector<char> &buf);
    void InsertSort(std::vector<Index> &index, Index inx);

    ~FileManager();
};
