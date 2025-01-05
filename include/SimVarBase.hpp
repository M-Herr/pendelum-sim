#pragma once

#include <string>


class SimVarBase
{
public:
    virtual ~SimVarBase() = default;

    // Optional: Each SimVar might have a name or ID for logging purposes.
    virtual std::string GetName() const = 0;  
};