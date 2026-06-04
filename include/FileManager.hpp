#pragma once
#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include "Student.hpp"
#include "Index.hpp"
#include <optional>

class FileManager
{
private:
    std::fstream in_out;
    std::optional<std::vector<Index>> loadIndex(std::string &indxFile);
    bool saveIndex(std::string &indxFile);
    std::vector<Index> index;
    std::string getIndxName(std::string fileName);

public:
    FileManager();

    std::vector<char> serializeStudent(const Student &st);
    Student deserializeStudent(const std::vector<char> &buf);
    void InsertSort(std::vector<Index> &index, Index inx);
    bool IndexManager(std::string &fileName); // If the user doesn't specify index file.
    bool IndexManager(std::string &fileName, std::string &fileIndxName);

    ~FileManager();
};
