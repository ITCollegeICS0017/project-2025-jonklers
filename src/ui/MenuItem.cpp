#include "MenuItem.h"

MenuItem::MenuItem(std::string label, std::vector<MenuItem> items, std::function<void()> action) : label(label), items(items), action(action) {}

std::string MenuItem::toString() const {
    std::string result;
    result += label;
    if (!items.empty()) result += "/"; // TODO: change folder display notation
    return result;
}
