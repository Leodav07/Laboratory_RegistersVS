
#include <cstdint>

struct Header
{
    uint32_t crc;
    uint32_t pageId;
    uint32_t freeSpace;
    uint32_t recordsCount;
};