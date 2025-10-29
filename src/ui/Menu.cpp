#include "Menu.h"

#include "utils.h"
#include <iostream>

Menu::Menu(std::shared_ptr<MenuItem> rootItem, std::vector<int> keys) : rootItem(rootItem), keys(keys) {};

void Menu::run() {
    while (isRunning) {
        clearConsole();
        display();
        handleInput();
    }
}
void Menu::handleInput() {
    Key key = detectKey();
    switch (key) {
        case Key::Up:
            selectPrev();
            break;
        case Key::Down:
            selectNext();
            break;
        case Key::Enter:
            select();
            break;
        case Key::Quit:
            exitMenu();
            break;
        default:
            break;
    }
}

void Menu::display() {
    displayLevel(getLevel());
}
void Menu::displayLevel(int level) {
    auto parent = getLevelParent(level);
    std::cout << parent->header;

    for (auto child : getCurrentChildren()) {
        if (child == getCurrentItem() ||
            (child->label == "Back" && getCurrentItem()->label == "Back") ||
            (child->label == "Exit" && getCurrentItem()->label == "Exit"))
            std::cout << "\x1b[4m" << child->toString() << "\x1b[0m" << std::endl;
        else
            std::cout << child->toString() << std::endl;
    }

    std::cout << parent->footer;
}

void Menu::exitMenu() {
    isRunning = false;
}
void Menu::backMenu() {
    if (getLevel() != 0) {
        keys.pop_back();
    }
    else {
        exitMenu();
    }
}
void Menu::gotoItem(std::shared_ptr<MenuItem> item) {
    keys = findItemPath(item);
}

int Menu::getLevel() {
    return keys.size() - 1;
}
int Menu::getLevelKey(int level) {
    return keys.at(level);
}
int Menu::getCurrentKey() {
    return getLevelKey(getLevel());
}

std::shared_ptr<MenuItem> Menu::getLevelParent(int level) {
    /*
     * WARNING: this function does not give dynamically generated items
     *          you should probably use Menu::getLevelChildren instead
     */

    std::shared_ptr<MenuItem> parent = rootItem;

    for (int i = 0; i < level; i++) {
        parent = parent->items.at(getLevelKey(i));
    }

    return parent;
}
std::vector<std::shared_ptr<MenuItem>> Menu::getLevelChildren(int level) {
    std::vector<std::shared_ptr<MenuItem>> items(getLevelParent(level)->items);

    if (level == 0)
        items.push_back(std::make_shared<MenuItem>(exitLabel, [this] { exitMenu(); }));
    else
        items.push_back(std::make_shared<MenuItem>(backLabel, [this] { backMenu(); }));

    return items;
}
std::vector<std::shared_ptr<MenuItem>> Menu::getCurrentChildren() {
    return getLevelChildren(getLevel());
}
std::shared_ptr<MenuItem> Menu::getCurrentItem() {
    return getLevelChildren(getLevel()).at(getCurrentKey());
}

std::vector<int> Menu::findItemPath(std::shared_ptr<MenuItem> target) {
    std::vector<int> _keys = rootItem->findItem(target);
    assert(!_keys.empty());
    return _keys;
}

void Menu::select() {
    auto item = getCurrentItem();
    if (item->action != nullptr) {
        item->action();
    }
    if (!item->items.empty()) {
        keys.push_back(0);
    }
}
void Menu::selectNext() {
    keys.back() = ++keys.back() % getCurrentChildren().size();
}
void Menu::selectPrev() {
    keys.back() = (--keys.back() + getCurrentChildren().size()) % getCurrentChildren().size();
}
