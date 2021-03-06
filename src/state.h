#pragma once
#include <vector>
#include <string>
#include <map>
#include "function.h"
#include "symbol.h"

/// Represents the execution state
/// Always push in forward order, and always pop in reverse order.
class State {
    /// Value stack
    std::vector<bool> m_stack;
    /// Maps names to functions
    std::map<std::string, FunctionPtr> m_functions;
    /// Offset pointer for variables
    size_t m_varOffset;
public:
    State();
    /// Get a function from name
    bool hasFunction(const std::string& name) const;
    /// Get a function from name
    Function& getFunction(const std::string& name);
    /// Get a constant function from name
    const Function& getFunction(const std::string& name) const;
    /// push value to stack
    void push(bool value);
    /// Pop value from stack
    bool pop();
    /// Set the variable offset. Returns the previous variable offset.
    size_t setVarOffset(size_t);
    /// Set a variable
    void setVar(size_t, bool);
    /// Get a variable
    bool getVar(size_t) const;
    /// Parse a file to create functions
    void parse(TokenBlock&& tokens);
    /// check this state for consistency and integrity
    /// will throw an exception if one of the following rules are broken:
    /// * inputs and outputs are mismatched in number
    /// * a variable is defined more than once
    /// * attempt to use an undefined variable
    /// * attempt to call an undefined function
    void check() const;
    /// Attempt to optimize functions within this state
    /// The goal of optimizing is generally to reduce the total number of
    /// operations performed, meaning fewer function calls, fewer
    /// expression/statement resolutions, and fewer stack operations.
    void optimize();
    /// Get number of values on stack
    size_t size() const;
    /// Resize the stack
    void resize(size_t);
    /// Get an integer value
    template <class T>
    T popValue();
    /// Put an integer value
    template <class T>
    void pushValue(T value);
};

template <class T>
T State::popValue()
{
    static const size_t bitsize = 8 * sizeof (T);
    static const T bitmask = T(1) << T(bitsize - 1);
    T value(0);
    for (size_t i = 0; i < bitsize; ++i) {
        value >>= 1;
        if (pop()) {
            value += bitmask;
        }
    }
    return value;
}

template <class T>
void State::pushValue(T value)
{
    static const size_t bitsize = 8 * sizeof (T);
    static const T bitmask = T(1) << T(bitsize - 1);
    for (size_t i = 0; i < bitsize; ++i) {
        // Most significant bit comes first, since this language behaves in
        // a big-endian way.
        bool b = value & bitmask;
        value <<= 1;
        push(b);
    }
}
