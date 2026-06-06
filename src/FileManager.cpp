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

    std::memcpy(p, st.account, sizeof(st.account));
    p += sizeof(st.account);

    std::copy(st.name.begin(), st.name.end(), p);
    p += st.name.length();

    std::memcpy(p, st.phoneNumber, sizeof(st.phoneNumber));
    p += sizeof(st.phoneNumber);

    *reinterpret_cast<uint32_t *>(p) = st.age;
    p += sizeof(uint32_t);

    std::memcpy(p, st.entryDate, sizeof(st.entryDate));
    p += sizeof(st.entryDate);

    *reinterpret_cast<bool *>(p) = st.isActive;

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

        st.isActive = *reinterpret_cast<const bool *>(p);

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

    while (i > 0 && memcmp(index[i - 1].account, index[i].account, sizeof(inx)) > 0)
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

    while (i > 0 && memcmp(index[i - 1].account, index[i].account, sizeof(Index)) > 0)
    {
        Index temp = index[i];
        index[i] = index[i - 1];
        index[i - 1] = temp;
        i--;
    }
}

std::optional<std::vector<Index>> FileManager::loadIndex(const std::string &indxFile)
{
    std::ifstream file(indxFile, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: The file ('" << indxFile << "') cannot open." << std::endl;
        return std::nullopt;
    }

    Index inx;
    std::vector<Index> index;

    while (file.read(reinterpret_cast<char *>(&inx), sizeof(inx)))
    {
        index.push_back(inx);
    }

    if (file.bad() || (file.fail() && !file.eof()))
    {
        std::cerr << "Format Error / Critical System Error." << std::endl;
        return std::nullopt;
    }

    return index;
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
    size_t dotPos = fileName.find_first_of('.');
    if (dotPos == std::string::npos)
    {
        return std::nullopt;
    }
    return fileName.substr(0, dotPos);
}

bool FileManager::IndexManager(std::string &fileName, std::vector<Index> &idx)
{
    auto indxNameFile = getIndxName(fileName);

    if (!indxNameFile.has_value())
    {
        std::cerr << "Error: Name of file is not valid. Please check and try again. (Expected .json)" << std::endl;
        return false;
    }

    auto index = loadIndex(indxNameFile.value());

    if (!index.has_value())
    {
        return false;
    }
    for (const auto &i : idx)
    {
        index.value().push_back(i);
    }
    InsertSort(index.value());

    return saveIndex(index.value(), indxNameFile.value());
}

bool FileManager::IndexManager(std::string &fileName, std::string &fileIndxName)
{
    auto index = loadIndex(fileIndxName);

    if (!index.has_value())
    {
        std::cerr << "Error: Name of file is not valid. Please check and try again. (Expected .idx)" << std::endl;
        return false;
    }

    InsertSort(index.value());
}

std::optional<std::vector<Student>> FileManager::jsonToStruct(std::string &jsonFile)
{
    try
    {
        nl::json studentsJson = nl::json::parse(jsonFile);

        std::vector<Student> students;

        if (studentsJson.is_array()) // First pass
        {
            for (auto &item : studentsJson)
            {

                if (item.contains("account") && item.at("account").is_string())
                {
                    std::string account = item.at("account").get<std::string>();
                    if (account.size() != 10)
                    {
                        std::cerr << "Error: The account number (" << account << ") doesn't have ten (10) characters exactly." << std::endl;
                        return std::nullopt;
                    }
                    for (int i = 0; i < realIndex.size(); i++)
                    {
                        if (account == std::string(realIndex[i].account, 10))
                        {
                            std::cerr << "Error: The account: " << account << " already exists. There can't exists duplicates." << std::endl;
                            return std::nullopt;
                        }
                    }
                }
                else
                {
                    return std::nullopt;
                }

                std::string account = item.at("account").get<std::string>();

                if (!item.contains("name") || !item.at("name").is_string())
                {
                    std::cerr << "Error: The student with account number: (" << account << ") does not contain 'name' field or 'name' field is not a string type." << std::endl;
                    return std::nullopt;
                }

                if (item.contains("phone") && item.at("phone").is_string())
                {
                    std::string phone = item.at("phone").get<std::string>();
                    if (phone.size() != 12)
                    {
                        std::cerr << "Error: The student account number (" << account << ") doesn't have twelve (12) characters exactly in 'phone' field." << std::endl;
                        return std::nullopt;
                    }
                }
                else
                {
                    return std::nullopt;
                }

                if (!item.contains("age") || !item.at("age").is_number_integer())
                {
                    std::cerr << "Error: The student with account number: (" << account << ") does not contain 'age' field or 'age' field is not an number_int type." << std::endl;
                    return std::nullopt;
                }

                if (item.contains("entryDate") && item.at("entryDate").is_string())
                {
                    std::string entryDate = item.at("entryDate").get<std::string>();
                    if (entryDate.size() != 8)
                    {
                        std::cerr << "Error: The student account number (" << account << ") doesn't have eight (8) characters exactly in 'entryDate' field." << std::endl;
                        return std::nullopt;
                    }

                    if (!isValidDateFormat(entryDate))
                    {
                        std::cerr << "Error: The student account number (" << account << ") has a format error in 'entryDate' field AAAAMMDD, please check." << std::endl;
                        return std::nullopt;
                    }
                }
                else
                {
                    return std::nullopt;
                }
            }

            for (auto &item : studentsJson)
            {
                Student st;
                std::string account = item.at("account").get<std::string>();
                std::memcpy(st.account, account.c_str(), account.size());
                st.name = item.at("name").get<std::string>();

                std::string phone = item.at("phone").get<std::string>();
                std::memcpy(st.phoneNumber, phone.c_str(), phone.size());

                st.age = item.at("age").get<uint32_t>(); // maybe can get error, so be careful

                std::string entryDate = item.at("entryDate").get<std::string>();
                std::memcpy(st.entryDate, entryDate.c_str(), entryDate.size());
            }
        }
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

bool FileManager::isValidDateFormat(std::string &date)
{
    for (auto &c : date)
    {
        if (std::isdigit(c))
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