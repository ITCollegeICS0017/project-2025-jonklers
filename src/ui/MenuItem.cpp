#include "MenuItem.h"

#include <string>
#include <vector>

MenuItem::MenuItem(std::string label, void (*action)()) : label(label), action(action) {}
MenuItem::MenuItem(std::string label, std::vector<MenuItem> items) : label(label), items(items) {}
MenuItem::MenuItem(std::string label, std::vector<MenuItem> items, void (*action)()) : label(label), items(items), action(action) {}

std::string MenuItem::toString() const {
    std::string result;
    result += label;
    if (!items.empty()) result += "/"; // TODO: change folder display notation
    return result;
}