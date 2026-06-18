#include "Header.hpp"

struct Page
{
    Header header;
    char data[512 - sizeof(Header)];
};