#include "brisk/logger.hpp"
#include "brisk/vector.hpp"

#include <vector>
#include <chrono>
#include <random>
#include <string>
#include <stdexcept>

static int convertStrToInt(const char* str)
{
    std::string a = str;
    return std::stoi(a, nullptr);
}

static int generateRandomInt(brisk::logger& c) noexcept
{
    try {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator;
        generator.seed(seed);
        return generator();

    } catch(std::exception& e) {
        c << "generateRandomInt(): caught exception -- " << e.what() << '\n';
        return 1;
    }
}

template <class T>
static void printInfoOfVector(const brisk::string& name, const brisk::vector<T>& v, brisk::logger& c) noexcept
{
    c << "\n\nVector Info:\nName: " << name << '\n'
    << "Size: " << v.size() << ", Size (in bytes): " << static_cast<size_t>(v.capacity() * sizeof(T)) << '\n' 
    << "Capacity: " << v.capacity() << ", Capacity (in bytes): " << static_cast<size_t>(v.capacity() * sizeof(T)) << '\n'
    << "Memory range: " << &v.front() << " - " << &v.back() << '\n'
    << "Elements: ";
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        c << *it << ' ';
    }
    c << '\n';
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

    cout << "Testing brisk::vector...";
    for (int i = 0; i < testRuns; i++) {
        if ((testRuns > 25) && ((i % (testRuns / 25)) == 0)) {
            cout << '.' << brisk::flush;
        }

        int numbers[5];
        for (int k = 0; k < 5; k++) {
            numbers[k] = generateRandomInt(cout);
        }
        
        start = system_clock::now();
        brisk::vector<int> v{1, 4, 16, 3, 5};
        for (int j = 0; j < operations; j++) {
            v.reserve(50);
            v.push_back(47);
            v.shrink_to_fit();
            v.resize(7);
            v.emplace_back(8);
            v.erase(v.begin() + 1);
            v.clear();
            brisk::vector<int> v2;
            v2.push_back(6);
            v = brisk::move(v2);
            v.shrink_to_fit();
            v.pop_back();
        }
        end = system_clock::now();
        briskElapsedSeconds += end - start;
    }

    cout << "\nTesting std::vector...";
    for (int i = 0; i < testRuns; i++) {
        if ((testRuns > 25) && ((i % (testRuns / 25)) == 0)) {
            cout << '.' << brisk::flush;
        }

        int numbers[5];
        for (int k = 0; k < 5; k++) {
            numbers[k] = generateRandomInt(cout);
        }

        start = system_clock::now();
        std::vector<int> v{1, 4, 16, 3, 5};
        for (int j = 0; j < operations; j++) {
            v.reserve(50);
            v.push_back(47);
            v.shrink_to_fit();
            v.resize(7);
            v.emplace_back(8);
            v.erase(v.begin() + 1);
            v.clear();
            std::vector<int> v2;
            v2.push_back(6);
            v = std::move(v2);
            v.shrink_to_fit();
            v.pop_back();
        }
        end = system_clock::now();
        stlElapsedSeconds += end - start;
    }
    
    cout << "\n\nStatistics:" << brisk::newl << 
    "Tests (for each container): "<< testRuns << brisk::newl <<
    "Operations (for each container, 8 operations each): " << operations << brisk::newl <<
    "Total operations performed (on each container): " << testRuns * (operations * 12) << brisk::newl <<
    "std::vector Time: " << stlElapsedSeconds.count() << "secs, brisk::vector Time: " << briskElapsedSeconds.count() << "secs" << brisk::newl <<
    "Total time elapsed: " << stlElapsedSeconds.count() + briskElapsedSeconds.count() << "secs" << brisk::newl <<
    "Press ENTER to quit..." << brisk::newl;
    
    std::cin.get();
}