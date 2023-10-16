#include "brisk/logger.hpp"
#include "brisk/string.hpp"

void getStringInfo(brisk::logger& c, const brisk::string& s)
{
    c << "String: " << s << ", Capacity: " << s.capacity() << ", Length (without NULL): " << s.length() << '\n';
    c << "Starting addr: " << (void*)&s[0] << ", Ending addr: " << (void*)&s[s.length()] << "\nContents (in log):\n";
    c.disablePrinting();
    for (int i = 0; i < s.capacity(); i++) {
        if (s[i] == 0) {
            c << "s[" << i << "]: NULL\n";
        } else {
            c << "s[" << i << "]: " << s[i] << '\n';
        }
    }
    c.enablePrinting();
}

int main()
{
    brisk::logger c("string_refactor.log");
    brisk::string s;
    c >> s;                 // Wow, this is now possible
    getStringInfo(c, s);
}