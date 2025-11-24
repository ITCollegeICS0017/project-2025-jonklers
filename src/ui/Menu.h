#pragma once

#include <vector>
#include <memory>
#include "MenuItem.h"

class Menu {
public:
    std::shared_ptr<MenuItem> rootItem;
    std::vector<int> keys;
    std::string backLabel = "Back";
    std::string exitLabel = "Exit";

    Menu(std::shared_ptr<MenuItem> rootItem, std::vector<int> keys = {0});

    void run();
    void handleInput();

    void display();
    void displayLevel(int level);

    void exitMenu();
    void backMenu();
    void gotoItem(std::shared_ptr<MenuItem> item);

private:
    bool isRunning = true;

    int getLevel();
    int getLevelKey(int level);
    int getCurrentKey();

    std::shared_ptr<MenuItem> getLevelParent(int level);
    std::vector<std::shared_ptr<MenuItem>> getLevelChildren(int level);
    std::vector<std::shared_ptr<MenuItem>> getCurrentChildren();
    std::shared_ptr<MenuItem> getCurrentItem();

    std::vector<int> findItemPath(std::shared_ptr<MenuItem> target);

    void select();
    void selectNext();
    void selectPrev();
};
