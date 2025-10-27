#include "Menu.h"

#include <vector>
#include <iostream>
#include "MenuItem.h"
#include "utils.h"

Menu::Menu(std::vector<MenuItem> items, bool horizontal, std::vector<int> keys) : items(items), keys(keys), horizontal(horizontal) {}

void Menu::displayHorizontal() const {
    for (int level = 0; level <= getLevel(); level++) {
        displayLevelHorizontal(level);
        std::cout << "\n";
    }
}

void Menu::run() {
    while (isRunning) {
        clearConsole();
        display();
        handleInput();
    }
}

void Menu::handleInput() {
    Key key = detectKey();
    if (horizontal) {
        switch (key) {
            case Key::Left:
                selectPrev();
                break;
            case Key::Right:
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
    } else {
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
}

void Menu::display() const {
    if (horizontal) {
        displayHorizontal();
    } else {
        displayVertical();
    }
}

void Menu::displayVertical() const {
    displayLevelVertical(getLevel());
}
void Menu::displayLevelHorizontal(int level) const {
    for (auto &item : getLevelItems(level)) {
        if (item.toString() == getCurrentLevelItem().toString()) // TODO: should fix comparison
        std::cout << "\x1b[4m" << item.toString() << "\x1b[0m ";
        else
        std::cout << item.toString() << " ";
    }
}
void Menu::displayLevelVertical(int level) const {
    for (auto &item : getLevelItems(level)) {
        if (item.toString() == getCurrentLevelItem().toString()) // TODO: should fix comparison
        std::cout << "\x1b[4m" << item.toString() << "\x1b[0m" << std::endl;
        else
        std::cout << item.toString() << std::endl;
    }
}

int Menu::getLevel() const {
    return keys.size() - 1;
}
int Menu::getLevelKey(int level) const {
    return keys.at(level);
}
int Menu::getCurrentLevelKey() const {
    return keys.back();
}
std::vector<MenuItem> Menu::getLevelItems(int level) const {
    if (level == 0) {
        return items;
    }
    MenuItem current = items.at(getLevelKey(0));
    for (int i = 1; i < level; i++) {
        current = current.items.at(getLevelKey(i));
    }
    return current.items;
}
std::vector<MenuItem> Menu::getCurrentLevelItems() const {
    return getLevelItems(getLevel());
}
MenuItem Menu::getCurrentLevelItem() const {
    return getCurrentLevelItems().at(getCurrentLevelKey());
}

void Menu::select() {
    if (getCurrentLevelItem().action != nullptr) {
        getCurrentLevelItem().action();
    } else if (!getCurrentLevelItem().items.empty()) {
        keys.push_back(0);
    }
}
void Menu::selectNext() {
    keys.back() = ++keys.back() % getCurrentLevelItems().size();
}
void Menu::selectPrev() {
    keys.back() = (--keys.back() + getCurrentLevelItems().size()) % getCurrentLevelItems().size();
}
void Menu::exitMenu() {
    isRunning = false;
}
void Menu::backMenu() {
    if (getLevel() > 0) {
        keys.pop_back();
    }
}
