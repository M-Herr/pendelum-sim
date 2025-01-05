#pragma once
#include <vector>
#include <mutex>
#include <string>
#include <optional>
#include <iostream>
#include <algorithm>

#include "SimVarBase.hpp"


class VariableRegistry
{
public:
    static VariableRegistry& Instance();

    void Add(SimVarBase* var);

    void Remove(SimVarBase* var);


    // Example: dump the name of each registered var
    void LogAll() const;

private:
    std::vector<SimVarBase*> vars_;
    mutable std::mutex mutex_;  // for thread safety
};