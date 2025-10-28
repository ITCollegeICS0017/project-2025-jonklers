#include "Menu.h"
#include "utils.h"

#include <iostream>

Menu::Menu(std::vector<MenuItem> items, bool horizontal, std::vector<int> keys) : items(items), horizontal(horizontal), keys(keys) {}

void Menu::run() {
    while (isRunning) {
        clearConsole();
        std::cout << items.size();
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

void Menu::display() {
    std::cout << headerMessage;
    if (horizontal) {
        displayHorizontal();
    } else {
        displayVertical();
    }
    std::cout << footerMessage;
}

void Menu::displayHorizontal() {
    for (int level = 0; level <= getLevel(); level++) {
        displayLevelHorizontal(level);
        std::cout << "\n";
    }
}
void Menu::displayVertical() {
    displayLevelVertical(getLevel());
}
void Menu::displayLevelHorizontal(int level) {
    for (auto &item : getLevelItems(level)) {
        if (item.toString() == getCurrentLevelItem().toString()) // TODO: should fix comparison
        std::cout << "\x1b[4m" << item.toString() << "\x1b[0m ";
        else
        std::cout << item.toString() << " ";
    }
}
void Menu::displayLevelVertical(int level) {
    for (auto &item : getLevelItems(level)) {
        if (item.toString() == getCurrentLevelItem().toString()) // TODO: should fix comparison
        std::cout << "\x1b[4m" << item.toString() << "\x1b[0m" << std::endl;
        else
        std::cout << item.toString() << std::endl;
    }
}

std::vector<MenuItem>* Menu::getLevelItemsRaw(int level) {
    std::vector<MenuItem>* itemsPtr = &items;

    for (int i = 0; i < level; i++) {
        itemsPtr = &(*itemsPtr).at(getLevelKey(i)).items;
    }

    return itemsPtr;
}

int Menu::getLevel() {
    return keys.size() - 1;
}
int Menu::getLevelKey(int level) {
    return keys.at(level);
}
int Menu::getCurrentLevelKey() {
    return keys.back();
}
std::vector<MenuItem> Menu::getLevelItems(int level) {
    std::vector<MenuItem> _items(*getLevelItemsRaw(level));

    if (level > 0) {
        MenuItem backItem(backLabel, {}, [this] { backMenu(); });
        _items.emplace_back(backItem);
    }

    MenuItem exitItem(exitLabel, {}, [this] { exitMenu(); });
    _items.emplace_back(exitItem);

    return _items;
}
std::vector<MenuItem> Menu::getCurrentLevelItems() {
    return getLevelItems(getLevel());
}
MenuItem Menu::getCurrentLevelItem() {
    return getCurrentLevelItems().at(getCurrentLevelKey());
}

void Menu::select() {
    auto item = getCurrentLevelItem();
    if (item.action != nullptr) {
        item.action();
    }
    if (!item.items.empty()) {
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
