#pragma once

#include <vector>
#include "MenuItem.h"

class Menu {
    public:
    std::vector<MenuItem> items;
    std::vector<int> keys;

    Menu(std::vector<MenuItem> items);

    void displayHorizontal() const;
    void displayVertical() const;
    void displayLevelHorizontal(int level) const;
    void displayLevelVertical(int level) const;

    void select();
    void selectNext();
    void selectPrev();
    void exitMenu();
    void backMenu();

    private:
    bool isRunning = true;
    
    int getLevel() const;
    int getLevelKey(int level) const;
    int getCurrentLevelKey() const;
    std::vector<MenuItem> getLevelItems(int level) const;
    std::vector<MenuItem> getCurrentLevelItems() const;
    MenuItem getCurrentLevelItem() const;
};