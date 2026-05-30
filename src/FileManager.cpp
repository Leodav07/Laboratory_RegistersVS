#include "FileManager.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <iostream>

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