#include "brisk/logger.hpp"
#include "brisk/array.hpp"

template <class T, size_t Size>
static void printContentsOfArray(brisk::logger& c, const brisk::string& s, const brisk::array<T, Size>& arr)
{
    c << "Size: " << arr.size() << '\n';
    c << "Contents:\n";
    for (typename brisk::array<T, Size>::const_iterator it = arr.cbegin(); it != arr.cend(); it++) {
        c << s << ": " << *it << '\n';
    }
}

int main(int argc, const char* argv[])
{
    brisk::logger c("array_swap_test.log");
    brisk::array<int, 5> a1{1, 2, 3, 4, 5}, a2{5, 6, 7, 8, 9};
    printContentsOfArray(c, "a1", a1);
    printContentsOfArray(c, "a2", a2);
    a2.swap(a1);
    printContentsOfArray(c, "a1", a1);
    printContentsOfArray(c, "a2", a2);
}