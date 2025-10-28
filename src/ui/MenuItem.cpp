#include "MenuItem.h"

MenuItem::MenuItem(std::string label, std::vector<std::shared_ptr<MenuItem>> items, std::function<void()> action) : label(label), items(items), action(action) {}

std::string MenuItem::toString() const {
    return label;
};