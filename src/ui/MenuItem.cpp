#include "MenuItem.h"

MenuItem::MenuItem(std::string label, std::vector<std::shared_ptr<MenuItem>> items, std::function<void()> action) : label(label), items(items), action(action) {}

std::string MenuItem::toString() const {
    return label;
};

std::shared_ptr<MenuItem> MenuItem::findItem(std::shared_ptr<MenuItem> target) {
    if (this == target.get()) {
        return shared_from_this();
    }

    for (auto child : items) {
        if (auto found = child->findItem(target)) {
            return found;
        }
    }

    return nullptr;
}
