#include "PageManager.hpp"
#include <cstring>
#include "cppcrc.h"

bool PageManager::pageControllerWrite(std::vector<char> &record, std::fstream &dFile)
{
    size_t fileSize = dFile.tellg();
    int totalPages = fileSize / 512;
    int pageId;
    if (totalPages == 0)
    {
        pageId = 0;
        page.header.pageId = 0;
        page.header.recordsCount = 0;
        page.header.freeSpace = 496;
    }
    else
    {
        pageId = totalPages - 1;
        dFile.seekg(pageId * 512, std::ios::beg);
        dFile.read(reinterpret_cast<char *>(&page), sizeof(page));

        if (page.header.freeSpace < record.size())
        {
            std::memset(&page, 0, sizeof(page));
            page.header.pageId = totalPages;
            page.header.freeSpace = 512 - sizeof(page.header);
            page.header.recordsCount = 0;
        }
    }

    int writePos = 496 - page.header.freeSpace;
    std::memcpy(page.data + writePos, record.data(), record.size());
    page.header.freeSpace -= record.size();
    page.header.recordsCount += 1;
    size_t dataSize = 512 - sizeof(page.header);
    page.header.crc = CRC32::CRC32::calc(reinterpret_cast<uint8_t *>(page.data), dataSize);
    absoluOffset = ((long long)page.header.pageId * 512) + sizeof(Header) + writePos;

    return true;
}