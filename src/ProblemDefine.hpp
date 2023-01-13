#pragma once

#include <functional>

using Data = double;
using Fn = std::function<Data(int)>;

bool greater_than_zero(Data x);
