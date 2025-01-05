#pragma once
#include <algorithm>
#include <vector>
#include <iostream>

namespace Utils 
{   
    template<typename T>
    void PushBackWithCheck(std::vector<T>& vec, T& thing_to_add)
    {
        auto it = std::find(vec.begin(), vec.end(), thing_to_add);

        if(it != vec.end())
        {
            //Found one already, don't add it
            std::cerr << "This thing exist already, can't add it.\n";
        }
        else 
        {
            //Not found, go ahead and add
            vec.push_back(thing_to_add);
        }

    }

    template<typename T>
    void EmplaceBackWithCheck(std::vector<T>& vec, T& thing_to_add)
    {
        auto it = std::find(vec.begin(), vec.end(), thing_to_add);

        if(it != vec.end())
        {
            //Found one already, don't add it
            std::cerr << "This thing exist already, can't add it.\n";
        }
        else 
        {
            //Not found, go ahead and add
            vec.emplace_back(thing_to_add);
        }

    }
}

