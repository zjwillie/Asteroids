// DronNode.hpp

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <variant>
#include <vector>

struct DronNode;

using DronList = std::vector<DronNode>;
using DronMap = std::map<std::string, DronNode>;

struct DronNode {
    std::variant<int32_t,
    double, bool,
    std::string,
    DronList,
    DronMap> value;
};

using DronDocument = DronMap;