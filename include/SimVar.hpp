#pragma once

#include "SimVarBase.hpp"
#include "VariableRegistry.hpp"
#include "type_traits.hpp"

template<typename T, bool EnableLogging = false>
class SimVar : public SimVarBase
{
    public:

    SimVar(std::string name, T initialValue = T{});

    SimVar() requires (!EnableLogging);

    //Copy constructor to do implicit conversions
    SimVar(const T& other);

    ~SimVar();

    // Accessors
    T& get();

    const T& get() const;

    void SetName(const std::string& name);

    template <typename U = T>
    SimVar& operator+=(const SimVar<U, EnableLogging>& rhs)
        requires SupportsAdditionAssignment<U>
    {
        mValue += rhs.mValue;
        return *this;
    }

    // -=
    template <typename U = T>
    SimVar& operator-=(const SimVar<U, EnableLogging>& rhs)
        requires SupportsSubtractionAssignment<U>
    {
        mValue -= rhs.mValue;
        return *this;
    }


    template <typename U = T>
    SimVar& operator++() requires SupportsIncrement<U>
    {
        ++mValue;
        return *this;
    }

    // postfix ++
    template <typename U = T>
    SimVar operator++(int) requires SupportsIncrement<U>
    {
        SimVar temp = *this;
        ++(*this);  // reuse prefix
        return temp;
    }


    template <typename U = T>
    SimVar& operator--() requires SupportsDecrement<U>
    {
        --mValue;
        return *this;
    }

    // postfix --
    template <typename U = T>
    SimVar operator--(int) requires SupportsDecrement<U>
    {
        SimVar temp = *this;
        --(*this);
        return temp;
    }

     SimVar& operator=(const T& rhs)
    {
        mValue = rhs;
        return *this;
    }

     template <typename U = T>
    friend SimVar<U, EnableLogging> operator+(const SimVar<U, EnableLogging>& lhs,
                                             const SimVar<U, EnableLogging>& rhs)
        requires SupportsAddition<U>
    {
        return { lhs.name_ + "+" + rhs.name_, lhs.mValue + rhs.mValue };
    }

    template <typename U = T>
    friend SimVar<U, EnableLogging> operator-(const SimVar<U, EnableLogging>& lhs,
                                             const SimVar<U, EnableLogging>& rhs)
        requires SupportsSubtraction<U>
    {
        return { lhs.name_ + "-" + rhs.name_, lhs.mValue - rhs.mValue };
    }


     template <typename U = T>
    friend SimVar<U, EnableLogging> operator*(const SimVar<U, EnableLogging>& lhs,
                                             const SimVar<U, EnableLogging>& rhs)
        requires SupportsMultiplication<U>
    {
        return { lhs.name_ + "*" + rhs.name_, lhs.mValue * rhs.mValue };
    }

     template <typename U = T>
    friend SimVar<U, EnableLogging> operator/(const SimVar<U, EnableLogging>& lhs,
                                             const SimVar<U, EnableLogging>& rhs)
        requires SupportsDivision<U>
    {
        return { lhs.name_ + "/" + rhs.name_, lhs.mValue / rhs.mValue };
    }

    template <typename U = T>
    friend SimVar<U, EnableLogging> operator%(const SimVar<U, EnableLogging>& lhs,
                                             const SimVar<U, EnableLogging>& rhs)
        requires SupportsModulo<U>
    {
        return { lhs.name_ + "%" + rhs.name_, lhs.mValue % rhs.mValue };
    }

    virtual std::string GetName() const override;


    private:
    std::string mName;

    T mValue;
};


template<typename T, bool EnableLogging>
SimVar<T, EnableLogging>::SimVar(std::string name, T initial_value)
: mName(std::move(name)), mValue(std::move(initial_value))
{
    if constexpr (EnableLogging) {
        VariableRegistry::Instance().Add(this);
    }
}

template<typename T, bool EnableLogging>
SimVar<T, EnableLogging>::SimVar() requires (!EnableLogging)
: mName("unnamed"), mValue()
{

}

template<typename T, bool EnableLogging>
SimVar<T, EnableLogging>::SimVar(const T& other)
{
    
}

template<typename T, bool EnableLogging>
SimVar<T, EnableLogging>::~SimVar()
{
    if constexpr (EnableLogging) {
        VariableRegistry::Instance().Remove(this);
    }
}


template <typename T, bool EnableLogging>
T& SimVar<T, EnableLogging>::get()
{
    return mValue;
}

template <typename T, bool EnableLogging>
const T& SimVar<T, EnableLogging>::get() const
{
    return mValue;
}

template<typename T, bool EnableLogging>
void SimVar<T, EnableLogging>::SetName(const std::string& name)
{
    mName = name;
}

template<typename T, bool EnableLogging>
std::string SimVar<T, EnableLogging>::GetName() const
{
    return mName;
}