
#include "Collection.hpp"

#include <random>

Collection<int> CallTest(size_t count)
{
    if (count == 0)
    {
        return Collection<int>(10000, 0);
    }

    Collection<int> result = CallTest(count - 1);

    static std::default_random_engine generator;
    const uint32_t index = generator() % result.size();
    for (size_t i = 0; i < index; ++i)
    {
        result[i] = count;
    }

    return result;
}

int main()
{
    { 
        Collection<int> b = CallTest(10);
    }
    std::cin.get();
    return 0;
}
