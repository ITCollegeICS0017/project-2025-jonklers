
#pragma once

#include <string>
#include <vector>

class MenuItem {
public:
    std::string label;
    std::vector<MenuItem> items;
    void (*action)() = nullptr;

    MenuItem(std::string label, void (*action)());
    MenuItem(std::string label, std::vector<MenuItem> items);
    MenuItem(std::string label, std::vector<MenuItem> items, void (*action)());

    std::string toString() const;
};