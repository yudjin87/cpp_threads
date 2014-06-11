#include <iostream>
#include <thread>
#include <boost/filesystem.hpp>

using namespace std;

int main()
{
    boost::filesystem::recursive_directory_iterator it("/home/devel");
    boost::filesystem::recursive_directory_iterator end;
    for ( ; it != end ; ++it)
    {
        if (boost::filesystem::is_regular_file(*it))
        {
            if(boost::filesystem::extension(*it) == ".cpp")
            {
                cout << *it << ":" << boost::filesystem::file_size(*it) << endl;
            }
        }
    }
}

