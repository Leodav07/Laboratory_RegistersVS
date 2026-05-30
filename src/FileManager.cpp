#include "FileManager.hpp"
#include <cstdint>
#include <cstring>

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

Student FileManager::deserializeStudent(std::vector<char> &buf)
{
}