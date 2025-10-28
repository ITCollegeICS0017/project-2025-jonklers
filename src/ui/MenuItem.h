#pragma once

#include <functional>
#include <vector>
#include <string>
#include <memory>

class MenuItem : public std::enable_shared_from_this<MenuItem> {
    public:
    std::string label;
    std::function<void()> action;
    std::vector<std::shared_ptr<MenuItem>> items;

    // When viewing children
    std::string header;
    std::string footer;

    explicit MenuItem(std::string label, std::vector<std::shared_ptr<MenuItem>> items = {}, std::function<void()> action = nullptr);

    std::string toString() const;

    std::shared_ptr<MenuItem> findItem(std::shared_ptr<MenuItem> target);
};
