#include "brisk/logger.hpp"
#include "brisk/vector.hpp"
#include "brisk/array.hpp"
#include "brisk/algorithm.hpp"

#include <chrono>
#include <thread>
#include <future>
#include <mutex>

template <class T>
static void printInfoOfVector(const brisk::string& name, const brisk::vector<T>& v, brisk::logger& c) noexcept
{
    c << "Name: " << name << '\n'
    << "Size: " << v.size() << ", Size (in bytes): " << static_cast<size_t>(v.capacity() * sizeof(T)) << '\n' 
    << "Capacity: " << v.capacity() << ", Capacity (in bytes): " << static_cast<size_t>(v.capacity() * sizeof(T)) << '\n'
    << "Memory range: " << &v.front() << " - " << &v.back() << '\n';
}

int main(int argc, const char* argv[])
{
    const size_t numbertron = 10000;
    const size_t numberOfThreads = 8;
    static brisk::logger c("c.log");
    brisk::vector<int> numbas(numbertron * numberOfThreads);
    std::mutex mtx;

    auto workerFunc = [&mtx, &numbas]() -> float {
        using namespace std::chrono;
        time_point<steady_clock> start = steady_clock::now();
        for (int i = 0; i < numbertron; i++) {
            mtx.lock();
            numbas.emplace_back(i);
            numbas.clear();
            mtx.unlock();
        }
        time_point<steady_clock> end = steady_clock::now();
        duration<float> timeElapsed(end - start);
        return duration_cast<milliseconds>(timeElapsed).count();
    };

    brisk::vector<std::future<float>> threads(numberOfThreads);
    brisk::for_each(threads.begin(), threads.end(), [&workerFunc](std::future<float>& future) {
        future = std::async(workerFunc);
    });

    brisk::for_each(threads.cbegin(), threads.cend(), [](const std::future<float>& future) {
        future.wait();
    });

    for (size_t i = 0; i < threads.size(); i++) {
        c << "Thread " << i << ": " << threads[i].get() << "ms\n";
    }

    printInfoOfVector("numbas", numbas, c);
}
