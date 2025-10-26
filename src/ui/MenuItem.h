
#pragma once

#include <string>
#include <vector>

class MenuItem {
public:
    std::string label;
    std::vector<MenuItem> items;
    void (*action)();

    MenuItem(const std::string label, const std::vector<MenuItem>* items, void (*action)());

    std::string toString() const;
};