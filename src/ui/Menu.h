#pragma once

#include <vector>
#include "MenuItem.h"

class Menu {
public:
    std::vector<MenuItem> items;
    std::vector<int> keys;
    bool horizontal;
    std::string headerMessage = "";
    std::string footerMessage = "";
    std::string backLabel = "Back";
    std::string exitLabel = "Exit";

    Menu(std::vector<MenuItem>& items, bool horizontal = true, std::vector<int> keys = {0});

    void run();
    void handleInput();

    void display();
    void displayHorizontal();
    void displayVertical();
    void displayLevelHorizontal(int level);
    void displayLevelVertical(int level);

    std::vector<MenuItem> getRawLevelItems(int level);

private:
    bool isRunning = true;
    
    int getLevel();
    int getLevelKey(int level);
    int getCurrentLevelKey();
    std::vector<MenuItem> getLevelItems(int level);
    std::vector<MenuItem> getCurrentLevelItems();
    MenuItem getCurrentLevelItem();

    void select();
    void selectNext();
    void selectPrev();
    void exitMenu();
    void backMenu();
};