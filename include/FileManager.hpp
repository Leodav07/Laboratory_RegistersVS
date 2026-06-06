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
    std::optional<std::vector<Index>> loadIndex(const std::string &indxFile);
    std::optional<std::string> getIndxName(std::string fileName);
    std::vector<Index> realIndex;
    bool isValidDateFormat(std::string &date);

public:
    FileManager();
    std::vector<char> serializeStudent(const Student &st);
    Student deserializeStudent(const std::vector<char> &buf);
    void InsertSort(std::vector<Index> &index, Index inx);
    void InsertSort(std::vector<Index> &index);
    bool IndexManager(std::string &fileName, std::vector<Index> &idx); // If the user doesn't specify index file.
    bool IndexManager(std::string &fileName, std::string &fileIndxName);
    bool saveIndex(std::vector<Index> &index, const std::string &indxFile);
    std::optional<std::vector<Student>> jsonToStruct(std::string &jsonFile);

    ~FileManager();
};
