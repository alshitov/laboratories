#include <new>

class DataLoaderSingleton
{
private:
    DataLoaderSingleton() = default;
    ~DataLoaderSingleton() = default;

    DataLoaderSingleton(const DataLoaderSingleton&) = delete;
    DataLoaderSingleton& operator=(const DataLoaderSingleton&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;

    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

public:
    static DataLoaderSingleton& getInstance()
    {
        static DataLoaderSingleton object;
        return object;
    }
};
