#include <cstdint>
#include <vector>
#include <fstream>
#include "Page.hpp"
#include <optional>

class PageManager
{
public:
    PageManager() {};

    bool pageControllerWrite(std::vector<char> &record, std::fstream &dFile);
    long long getAbsoluOffset()
    {
        return absoluOffset;
    }
    std::optional<std::vector<char>> pageControllerRead(long long offset, std::fstream &dFile);
    bool markInactivePage(long long offset, std::fstream &dFile);

private:
    Page page;
    bool verifyCRC(long long offset, std::fstream &dFile);
    long long absoluOffset;
};