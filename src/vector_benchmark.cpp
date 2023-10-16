#include "brisk/logger.hpp"
#include "brisk/vector.hpp"

#include <vector>
#include <chrono>
#include <string>
#include <stdexcept>

static int convertStrToInt(const char* str)
{
    std::string a = str;
    return std::stoi(a, nullptr);
}

int main(int argc, const char* argv[])
{
    brisk::logger cout("cout.log");

    using namespace std::chrono;
    time_point<system_clock> start, end;
    duration<float> briskElapsedSeconds, stlElapsedSeconds = duration<float>::zero();
    int testRuns, operations;
    bool useDefaults = true;

    if (argc >= 3)
    {
        try {
            testRuns = convertStrToInt(argv[1]);
            operations = convertStrToInt(argv[2]);

            if (argc > 3) {
                cout.filename(argv[3]);
            }

            useDefaults = false;
        } catch(std::exception& e) {
            cout << "[ERROR]" << e.what() << '\n';
            useDefaults = true;
        }
    }

    if (useDefaults) {
        testRuns = 100;
        operations = 100000;
    }

    cout << "Testing std::vector...";
    for (int i = 0; i < testRuns; i++) {
        if ((testRuns > 25) && ((i % (testRuns / 25)) == 0)) {
            cout << '.';
        }

        start = system_clock::now();
        std::vector<int> v{1, 2, 3, 4, 5};
        for (int j = 0; j < operations; j++) {
            v.push_back(256);
            v.push_back(512);
            v.push_back(1024);
            v.push_back(2048);
            v.shrink_to_fit();
            v.resize(3);
            v.emplace_back(2);
            v.erase(v.end() - 1);
        }
        end = system_clock::now();
        stlElapsedSeconds += end - start;
    }

    cout << "\nTesting brisk::vector...";
    for (int i = 0; i < testRuns; i++) {
        if ((testRuns > 25) && ((i % (testRuns / 25)) == 0)) {
            cout << '.';
        }
        
        start = system_clock::now();
        brisk::vector<int> v{1, 2, 3, 4, 5};
        for (int j = 0; j < operations; j++) {
            v.push_back(256);
            v.push_back(512);
            v.push_back(1024);
            v.push_back(2048);
            v.shrink_to_fit();
            v.resize(3);
            v.emplace_back(2);
            v.erase(v.end() - 1);
        }
        end = system_clock::now();
        briskElapsedSeconds += end - start;
    }
    
    cout << "\n\nStatistics:" << brisk::newl << 
    "Tests (for each container): "<< testRuns << brisk::newl <<
    "Operations (for each container, 8 operations each): " << operations << brisk::newl <<
    "Total operations performed (on each container): " << testRuns * (operations * 8) << brisk::newl <<
    "std::vector Time: " << stlElapsedSeconds.count() << "secs, brisk::vector Time: " << briskElapsedSeconds.count() << "secs" << brisk::newl <<
    "Total time elapsed: " << stlElapsedSeconds.count() + briskElapsedSeconds.count() << "secs" << brisk::newl <<
    "Press ENTER to quit..." << brisk::newl;
    
    std::cin.get();
}