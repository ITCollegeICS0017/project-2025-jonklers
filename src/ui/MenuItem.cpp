#include "MenuItem.h"

MenuItem::MenuItem(std::string label, std::function<void()> action) : label(label), items({}), action(action) {}
MenuItem::MenuItem(std::string label, std::vector<std::shared_ptr<MenuItem>> items, std::function<void()> action) : label(label), items(items), action(action) {}

std::string MenuItem::toString() const {
    return label;
};

std::vector<int> MenuItem::findItem(std::shared_ptr<MenuItem> target, std::vector<int> keys) {
    if (this == target.get()) {
        return keys;
    }

    keys.push_back(0);
    for (std::shared_ptr<MenuItem> child : items) {
        std::vector<int> found = child->findItem(target, keys);
        if (!found.empty()) {
            return found;
        }
        keys.back++;
    }

    return {};
}
