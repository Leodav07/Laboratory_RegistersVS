#pragma once
#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include "Student.hpp"
#include "Index.hpp"
#include <optional>
#include "json.hpp"
namespace nl = nlohmann;

class FileManager
{
private:
    bool loadIndex(const std::string &indxFile);
    std::optional<std::string> getIndxName(std::string fileName);
    std::vector<Index> realIndex;
    bool isValidDateFormat(std::string &date);
    bool verifyJSON(nl::json &item, bool isUpdate = false);
    std::optional<Index> binarySearch(const std::string &accountSt);
    bool markInactive(std::fstream &file, long long offset);

public:
    FileManager() {};
    std::vector<char> serializeStudent(const Student &st);
    Student deserializeStudent(const std::vector<char> &buf);
    void InsertSort(std::vector<Index> &index, Index inx);
    void InsertSort(std::vector<Index> &index);
    bool saveIndex(std::vector<Index> &index, const std::string &indxFile);
    std::optional<std::vector<Student>> jsonToStruct(const std::string &jsonFile, bool isUpdate = false);
    bool addRecords(const std::string &jsonFile, const std::string &dataFile, const std::string &indexFile);
    std::optional<Student> searchByCode(const std::string &dataFile, const std::string &codeAccount, const std::string &indexFile);
    bool removeStudentByCode(const std::string &dataFile, const std::string &codeAccount, const std::string &indexFile);
    bool updateStudent(const std::string &dataFile, const std::string &jsonFile, const std::string &indexFile);
    bool cleanUp(const std::string &dataFile, const std::string &indexFile);
    ~FileManager() {};
};
