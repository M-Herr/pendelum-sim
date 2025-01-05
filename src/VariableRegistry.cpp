#include "VariableRegistry.hpp"

VariableRegistry &Instance()
{
    static VariableRegistry theRegistry;
    return theRegistry;
}

void VariableRegistry::Add(SimVarBase *var)
{
    std::lock_guard<std::mutex> lock(mutex_);
    vars_.push_back(var);
}

void VariableRegistry::Remove(SimVarBase *var)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find(vars_.begin(), vars_.end(), var);
    if (it != vars_.end())
        vars_.erase(it);
}

void VariableRegistry::LogAll() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto *v : vars_)
        std::cout << v->GetName() << "\n";
}