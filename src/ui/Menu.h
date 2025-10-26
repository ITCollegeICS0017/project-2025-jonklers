#pragma once

#include <vector>
#include "MenuItem.h"

class Menu {
    public:
    std::vector<MenuItem> items;
    std::vector<int> keys;

    Menu(const std::vector<MenuItem>* items, const std::vector<int> keys);

    void displayHorizontal(int level) const;
    void displayVertical(int level) const;
    void displayLevelHorizontal(int level) const;
    void displayLevelVertical(int level) const;

    private:
    bool isRunning;

    int getTotalLevels() const;
    int getCurrentLevel() const;
    int getLevelKey(int level) const;
    int getCurrentLevelKey() const;
    std::vector<MenuItem> getLevelItems(int level) const;
    std::vector<MenuItem> getCurrentLevelItems() const;

    void select();
    void selectNext();
    void selectPrev();
    void exitMenu();
    void backMenu();
};