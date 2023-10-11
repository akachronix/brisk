#include "brisk/logger.hpp"
#include "brisk/string.hpp"
#include "brisk/memory.hpp"
#include "brisk/vector.hpp"

#include <cstdlib>
#include <ctime>

template <typename T>
void printContentsOfVector(brisk::logger& c, const brisk::vector<T>& v, const char* name)
{
    c << name << " elements: " << v.size() << ", " << name << " capacity: " << v.capacity() << '\n';
    for (auto it = v.cbegin(); it != v.cend(); it++)
    {
        c << name << "[" << static_cast<int>(it - v.cbegin()) << "]: " << *it << '\n';
    }
}

int main(void)
{
    srand(static_cast<unsigned>(time(0)));
    brisk::logger cout(brisk::loglevel::everything, "cout.log");

    brisk::vector<std::string> strings;
    strings.emplace_back("cum");
    strings.emplace_back("dump");
    strings.emplace_back("dumb");
    strings.emplace_back("dumpster");
    strings.emplace_back("dumpy");

    printContentsOfVector(cout, strings, "strings");
}