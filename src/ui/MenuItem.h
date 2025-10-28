#pragma once
#include <functional>
#include <vector>
#include <string>

class MenuItem {
    public:
    std::string label;
    std::function<void()> action;
    std::vector<MenuItem> items;

    explicit MenuItem(std::string label, std::vector<MenuItem> items = {}, std::function<void()> action = nullptr);

    std::string toString() const;
};
