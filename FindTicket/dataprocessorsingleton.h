#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H
#include <new>
#include <loader.h>

class DataProcessorSingleton
{
private:
    DataProcessorSingleton() = default;
    ~DataProcessorSingleton() = default;

    DataProcessorSingleton(const DataProcessorSingleton&) = delete;
    DataProcessorSingleton& operator=(const DataProcessorSingleton&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;

    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

public:
    static DataProcessorSingleton& getInstance()
    {
        static DataProcessorSingleton object;
        return object;
    }
};

#endif // DATAPROCESSOR_H
