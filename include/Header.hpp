
#include <cstdint>

struct Header
{
    uint16_t crc;
    uint32_t pageId;
    uint32_t recordsCount;
    uint32_t freeSpace;
};