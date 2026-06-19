#include <cstdint>
#include <vector>
#include <fstream>
#include "Page.hpp"


class PageManager
{
public:
    PageManager() {};

    bool pageControllerWrite(std::vector<char> &record, std::fstream &dFile);
    long long getAbosoluOffset()
    {
        return absoluOffset;
    }

private:
    Page page;
    long long absoluOffset;
};