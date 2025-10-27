
#pragma once

#include <string>
#include <vector>
#include <functional>

class MenuItem {
public:
    std::string label;
    std::vector<MenuItem> items;
    std::function<void()> action = nullptr;

    MenuItem(std::string label, std::function<void()> action);
    MenuItem(std::string label, std::vector<MenuItem> items);
    MenuItem(std::string label, std::vector<MenuItem> items, std::function<void()> action);

    std::string toString() const;
};