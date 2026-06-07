#include "FileManager.hpp"
#include <cstring>
#include <iostream>
#include <json.hpp>

namespace nl = nlohmann;

std::vector<char> FileManager::serializeStudent(const Student &st)
{
    size_t totalSize = sizeof(st.account) + st.name.length() + sizeof(st.phoneNumber) +
                       sizeof(uint32_t) + sizeof(st.entryDate) + sizeof(bool);

    std::vector<char> record(totalSize + sizeof(uint32_t));
    char *p = record.data();

    *reinterpret_cast<uint32_t *>(p) = totalSize;
    p += sizeof(uint32_t);

    *reinterpret_cast<bool *>(p) = st.isActive;
    p += sizeof(bool);

    std::memcpy(p, st.account, sizeof(st.account));
    p += sizeof(st.account);

    std::copy(st.name.begin(), st.name.end(), p);
    p += st.name.length();

    std::memcpy(p, st.phoneNumber, sizeof(st.phoneNumber));
    p += sizeof(st.phoneNumber);

    *reinterpret_cast<uint32_t *>(p) = st.age;
    p += sizeof(uint32_t);

    std::memcpy(p, st.entryDate, sizeof(st.entryDate));

    return record;
}

Student FileManager::deserializeStudent(const std::vector<char> &buf)
{
    try
    {
        if (buf.size() < sizeof(uint32_t))
        {
            throw std::runtime_error("The buffer is very short to read header.");
        }
        const char *p = buf.data();
        uint32_t totalSize = *reinterpret_cast<const uint32_t *>(p);
        p += sizeof(uint32_t);

        if (totalSize < (sizeof(Student::account) + sizeof(Student::phoneNumber) +
                         sizeof(uint32_t) + sizeof(Student::entryDate) + sizeof(bool)))
        {
            throw std::runtime_error("TotalSize doesn't have the minimum size required.");
        }

        if (buf.size() < totalSize + sizeof(uint32_t))
        {
            throw std::runtime_error("Buffer is incomplete according to TotalSize.");
        }

        Student st;
        st.isActive = *reinterpret_cast<const bool *>(p);
        p += sizeof(bool);
        std::memcpy(st.account, p, sizeof(st.account));
        p += sizeof(st.account);

        size_t nameLength = totalSize - (sizeof(Student::account) + sizeof(Student::phoneNumber) +
                                         sizeof(uint32_t) + sizeof(Student::entryDate) + sizeof(bool));
        std::string name(p, nameLength);
        st.name = name;
        p += nameLength;

        std::memcpy(st.phoneNumber, p, sizeof(st.phoneNumber));
        p += sizeof(st.phoneNumber);

        st.age = *reinterpret_cast<const uint32_t *>(p);
        p += sizeof(uint32_t);

        std::memcpy(st.entryDate, p, sizeof(st.entryDate));
        p += sizeof(st.entryDate);

        return st;
    }
    catch (std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
}

void FileManager::InsertSort(std::vector<Index> &index, Index inx)
{
    index.push_back(inx);
    size_t i = index.size() - 1;

    while (i > 0 && memcmp(index[i - 1].account, index[i].account, sizeof(Index::account)) > 0)
    {
        Index temp = index[i];
        index[i] = index[i - 1];
        index[i - 1] = temp;
        i--;
    }
}

void FileManager::InsertSort(std::vector<Index> &index)
{
    size_t i = index.size() - 1;

    while (i > 0 && memcmp(index[i - 1].account, index[i].account, sizeof(Index::account)) > 0)
    {
        Index temp = index[i];
        index[i] = index[i - 1];
        index[i - 1] = temp;
        i--;
    }
}

bool FileManager::loadIndex(const std::string &indxFile)
{
    realIndex.clear();
    std::ofstream createIfNotExists(indxFile, std::ios::app);
    createIfNotExists.close();

    std::ifstream file(indxFile, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: The file ('" << indxFile << "') cannot open." << std::endl;
        return false;
    }

    Index inx;

    while (file.read(reinterpret_cast<char *>(&inx), sizeof(inx)))
    {
        realIndex.push_back(inx);
    }

    if (file.bad() || (file.fail() && !file.eof()))
    {
        std::cerr << "Format Error / Critical System Error." << std::endl;
        return false;
    }

    return true;
}

bool FileManager::saveIndex(std::vector<Index> &index, const std::string &indxFile)
{
    std::ofstream file(indxFile, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error: The file ('" << indxFile << "') cannot open." << std::endl;
        return false;
    }

    for (const auto &indx : index)
    {
        file.write(reinterpret_cast<const char *>(&indx), sizeof(indx));
    }

    if (!file.good())
    {
        std::cerr << "Critical Error: Index saving operation failed." << std::endl;
        return false;
    }
    return true;
}

std::optional<std::string> FileManager::getIndxName(std::string fileName)
{
    size_t dotPos = fileName.rfind('.');
    if (dotPos == std::string::npos)
    {
        return std::nullopt;
    }
    return fileName.substr(0, dotPos);
}

std::optional<std::vector<Student>> FileManager::jsonToStruct(const std::string &jsonFile, bool isUpdate)
{
    try
    {
        std::ifstream file(jsonFile);
        if (!file.is_open())
        {
            std::cerr << "Error: cannot open JSON file." << std::endl;
            return std::nullopt;
        }
        nl::json studentsJson = nl::json::parse(file);

        std::vector<Student> students;

        if (studentsJson.is_array()) // First pass
        {
            for (auto &item : studentsJson)
            {
                if (!verifyJSON(item, isUpdate))
                    return std::nullopt;
            }

            for (auto &item : studentsJson)
            {
                Student st;
                std::string account = item.at("account").get<std::string>();
                std::memset(st.account, 0, sizeof(st.account));
                std::memcpy(st.account, account.c_str(), account.size());
                st.name = item.at("name").get<std::string>();

                std::string phone = item.at("phone").get<std::string>();
                std::memset(st.phoneNumber, 0, sizeof(st.phoneNumber));
                std::memcpy(st.phoneNumber, phone.c_str(), phone.size());

                st.age = item.at("age").get<uint32_t>(); // maybe can get error, so be careful

                std::string entryDate = item.at("entryDate").get<std::string>();
                std::memset(st.entryDate, 0, sizeof(st.entryDate));
                std::memcpy(st.entryDate, entryDate.c_str(), entryDate.size());
                students.push_back(st);
            }
        }
        else
        {
            if (!verifyJSON(studentsJson, isUpdate))
            {
                return std::nullopt;
            }
            else
            {
                Student st;
                std::string account = studentsJson.at("account").get<std::string>();
                std::memset(st.account, 0, sizeof(st.account));
                std::memcpy(st.account, account.c_str(), account.size());
                st.name = studentsJson.at("name").get<std::string>();

                std::string phone = studentsJson.at("phone").get<std::string>();
                std::memset(st.phoneNumber, 0, sizeof(st.phoneNumber));
                std::memcpy(st.phoneNumber, phone.c_str(), phone.size());

                st.age = studentsJson.at("age").get<uint32_t>(); // maybe can get error, so be careful

                std::string entryDate = studentsJson.at("entryDate").get<std::string>();
                std::memset(st.entryDate, 0, sizeof(st.entryDate));
                std::memcpy(st.entryDate, entryDate.c_str(), entryDate.size());
                students.push_back(st);
            }
        }
        return students;
    }
    catch (nl::json::parse_error &p)
    {
        std::cerr << "Error: Parse error in byte: " << p.byte << '\n'
                  << "Description: " << p.what() << std::endl;
        return std::nullopt;
    }
    catch (nl::json::type_error &t)
    {
        std::cerr << "Error: Invalid type error" << '\n'
                  << "Description: " << t.what() << std::endl;
        return std::nullopt;
    }
    catch (nl::json::other_error &o)
    {
        std::cerr << "Error" << '\n'
                  << "Description: " << o.what() << std::endl;
        return std::nullopt;
    }
    catch (nl::json::exception &je)
    {
        std::cerr << "Error: Json exception" << '\n'
                  << "Description: " << je.what() << std::endl;
        return std::nullopt;
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: Exception" << '\n'
                  << "Description: " << e.what() << std::endl;
        return std::nullopt;
    }
}
bool FileManager::verifyJSON(nl::json &item, bool isUpdate)
{

    if (item.contains("account") && item.at("account").is_string())
    {
        std::string account = item.at("account").get<std::string>();
        if (account.size() != 10)
        {
            std::cerr << "Error: The account number (" << account << ") doesn't have ten (10) characters exactly." << std::endl;
            return false;
        }
        auto idx = binarySearch(account);
        if (!isUpdate && idx.has_value())
        {
            std::cerr << "Error: The account: " << account << " already exists. There can't exists duplicates." << std::endl;
            return false;
        }
        else if (isUpdate && !idx.has_value())
        {
            std::cerr << "Error: The account: " << account << " doesn't exist." << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }

    std::string account = item.at("account").get<std::string>();

    if (!item.contains("name") || !item.at("name").is_string())
    {
        std::cerr << "Error: The student with account number: (" << account << ") does not contain 'name' field or 'name' field is not a string type." << std::endl;
        return false;
    }

    if (item.contains("phone") && item.at("phone").is_string())
    {
        std::string phone = item.at("phone").get<std::string>();
        if (phone.size() != 12)
        {
            std::cerr << "Error: The student account number (" << account << ") doesn't have twelve (12) characters exactly in 'phone' field." << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }

    if (!item.contains("age") || !item.at("age").is_number_integer())
    {
        std::cerr << "Error: The student with account number: (" << account << ") does not contain 'age' field or 'age' field is not an number_int type." << std::endl;
        return false;
    }

    if (item.contains("entryDate") && item.at("entryDate").is_string())
    {
        std::string entryDate = item.at("entryDate").get<std::string>();
        if (entryDate.size() != 8)
        {
            std::cerr << "Error: The student account number (" << account << ") doesn't have eight (8) characters exactly in 'entryDate' field." << std::endl;
            return false;
        }

        if (!isValidDateFormat(entryDate))
        {
            std::cerr << "Error: The student account number (" << account << ") has a format error in 'entryDate' field AAAAMMDD, please check." << std::endl;
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool FileManager::isValidDateFormat(std::string &date)
{
    for (auto &c : date)
    {
        if (!std::isdigit(c))
            return false;
    }

    int year = std::stoi(date.substr(0, 4));
    if (year < 1910)
        return false;
    int month = std::stoi(date.substr(4, 2));
    if (month > 12 || month < 1)
        return false;
    int day = std::stoi(date.substr(6, 2));
    if ((day > 31 || day < 1))
        return false;

    return true;
}

bool FileManager::addRecords(const std::string &jsonFile, const std::string &dataFile, const std::string &indexFile)
{
    std::ofstream dFile(dataFile, std::ios::binary | std::ios::app);

    if (!dFile.is_open())
    {
        std::cerr << "The file (" << dataFile << ") cannot open." << std::endl;
        return false;
    }

    if (!loadIndex(indexFile))
    {
        std::ifstream check(indexFile);
        if (check.good())
        {
            std::cerr << "Error: error loading index." << std::endl;
            return false;
        }
    }

    auto students = jsonToStruct(jsonFile);

    if (!students.has_value())
    {
        std::cerr << "Error getting value." << std::endl;
        return false;
    }

    for (const auto &item : students.value())
    {
        Index idx;
        std::vector<char> record;
        long long offset = dFile.tellp();
        record = serializeStudent(item);
        dFile.write(record.data(), record.size());

        std::memset(idx.account, 0, sizeof(idx.account));
        std::memcpy(idx.account, item.account, sizeof(item.account));

        idx.offset = offset;
        idx.size = record.size();
        idx.isActive = true;

        InsertSort(realIndex, idx);
    }

    if (!saveIndex(realIndex, indexFile))
    {
        std::cerr << "Error: index saving proccess failed." << std::endl;
        return false;
    }
    return true;
}

std::optional<Index> FileManager::binarySearch(const std::string &accountSt)
{
    int low = 0;
    int high = realIndex.size() - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2;
        std::string midAccount(realIndex[mid].account, sizeof(Index::account));
        if (midAccount == accountSt)
        {
            return realIndex[mid];
        }

        if (midAccount < accountSt)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return std::nullopt;
}

std::optional<Student> FileManager::searchByCode(const std::string &dataFile, const std::string &codeAccount, const std::string &indexFile)
{
    std::ifstream dFile(dataFile, std::ios::binary);

    if (!dFile.is_open())
    {
        std::cerr << "The file (" << dataFile << ") cannot open." << std::endl;
        return std::nullopt;
    }

    if (!loadIndex(indexFile))
    {
        std::ifstream check(indexFile);
        if (check.good())
        {
            std::cerr << "Error: error loading index." << std::endl;
            return std::nullopt;
        }
    }

    auto idxSt = binarySearch(codeAccount);

    if (!idxSt.has_value() || !idxSt.value().isActive)
    {
        std::cerr << "Student with " << codeAccount << " code doesn't exist or is inactive." << std::endl;
        return std::nullopt;
    }

    long long offset = idxSt.value().offset;
    Student st;
    uint32_t tSize;
    dFile.seekg(offset, std::ios::beg);
    dFile.read(reinterpret_cast<char *>(&tSize), sizeof(tSize));

    std::vector<char> buf(tSize + sizeof(uint32_t));

    dFile.seekg(offset, std::ios::beg);
    dFile.read(buf.data(), buf.size());

    return deserializeStudent(buf);
}

bool FileManager::removeStudentByCode(const std::string &dataFile, const std::string &codeAccount, const std::string &indexFile)
{
    std::fstream dFile(dataFile, std::ios::binary | std::ios::in | std::ios::out);

    if (!dFile.is_open())
    {
        std::cerr << "The file (" << dataFile << ") cannot open." << std::endl;
        return false;
    }

    if (!loadIndex(indexFile))
    {
        std::ifstream check(indexFile);
        if (check.good())
        {
            std::cerr << "Error: error loading index." << std::endl;
            return false;
        }
    }

    auto st = binarySearch(codeAccount);

    if (st.has_value() && !st.value().isActive)
    {
        std::cerr << "This student record has been eliminated before." << std::endl;
        return false;
    }
    else if (!st.has_value())
    {
        std::cerr << "Student with " << codeAccount << " code doesn't exist." << std::endl;
        return false;
    }

    long long offset = st.value().offset;

    bool inactive = false;
    dFile.seekp(offset + sizeof(uint32_t), std::ios::beg);
    if (!dFile.write(reinterpret_cast<const char *>(&inactive), sizeof(bool)))
    {
        std::cerr << "Error while overwriting record status." << std::endl;
        return false;
    }

    for (auto &idx : realIndex)
    {
        std::string idxAccount(idx.account, sizeof(idx.account));
        if (idxAccount == codeAccount)
        {
            idx.isActive = false;
        }
    }

    return saveIndex(realIndex, indexFile);
}

bool FileManager::updateStudent(const std::string &dataFile, const std::string &jsonFile, const std::string &indexFile)
{
    if (!loadIndex(indexFile))
    {
        std::ifstream check(indexFile);
        if (check.good())
        {
            std::cerr << "Error: error loading index." << std::endl;
            return false;
        }
    }

    auto students = jsonToStruct(jsonFile, true);
    std::fstream dFile(dataFile, std::ios::binary | std::ios::in | std::ios::out);

    if (!dFile.is_open())
    {
        std::cerr << "The file (" << dataFile << ") cannot open." << std::endl;
        return false;
    }

    if (!students.has_value())
    {
        std::cerr << "Error" << std::endl;
        return false;
    }

    for (auto &st : students.value())
    {
        std::string codeAccount(st.account, sizeof(st.account));
        auto index = binarySearch(codeAccount);

        if (!index.has_value())
        {
            std::cerr << "Error searching index..." << std::endl;
            return false;
        }

        if (!markInactive(dFile, index.value().offset))
        {
            std::cerr << "Error ocurred with student account: " << codeAccount << std::endl;
            return false;
        }
        std::vector<char> newRecord = serializeStudent(st);
        dFile.seekp(0, std::ios::end);
        long long nOffSet = dFile.tellp();
        if (!dFile.write(newRecord.data(), newRecord.size()))
        {
            std::cerr << "Error while writing update record of: " << codeAccount << std::endl;
            return false;
        }

        for (auto &idx : realIndex)
        {
            std::string idxAccount(idx.account, sizeof(idx.account));
            if (idxAccount == codeAccount)
            {
                idx.offset = nOffSet;
                idx.size = newRecord.size();
            }
        }
    }

    return saveIndex(realIndex, indexFile);
}

bool FileManager::markInactive(std::fstream &file, long long offset)
{
    file.seekp(offset + sizeof(uint32_t), std::ios::beg);
    bool inactive = false;
    return file.write(reinterpret_cast<const char *>(&inactive), sizeof(inactive)).good();
}