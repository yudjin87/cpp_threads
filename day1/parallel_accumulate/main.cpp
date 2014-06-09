#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<typename It, typename T>
T parallel_accumulate(It begin, It end, T init)
{
    return init;
}

int main()
{
    const size_t BIG = 1000000;
    cout << "Hello World!" << endl;
    vector<unsigned long> vec;
    for (int i = 0 ; i < BIG ; ++i)
    {
        vec.push_back(i);
    }

    cout << "Just the Sum = " << accumulate(vec.begin(), vec.end(), 0L) << endl;
    cout << "Parallel Sum = " << parallel_accumulate(vec.begin(), vec.end(), 0L) << endl;

    return 0;
}

