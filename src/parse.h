#pragma once
#include "symbol.h"
#include "debug.h"
#include <vector>
#include <iostream>

/// Takes an input stream and parses it into tokens.
TokenBlock parseTokens(std::istream& stream, DebugInfo info);
