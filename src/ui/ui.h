#pragma once

#include <string>
#include <vector>

using namespace std;

class MenuItem {
    public:
    string label;
    vector<MenuItem> items;
    void (*action)();

    MenuItem(const vector<MenuItem> items, void (*action)()) : items(items), action(action) {}

    string toString() {}
};

class Menu {
    public:
    vector<MenuItem> items;
    vector<int> keys;

    Menu(const vector<MenuItem> items, const vector<int> keys, void (*exitAction)()) : items(items), keys(keys), exitAction(exitAction) {}

    void runHorizontal() {}
    void runVertical() {}

    private:
    bool isRunning;

    int getCurrentLevel() {}
    int getLevelKey(int level) {}
    vector<MenuItem> getLevelItems(int level) {}
    vector<MenuItem> getCurrentItems() {}

    void select() {}
    void selectNext() {}
    void selectPrev() {}
    void exitMenu() {}
    void backMenu() {}
};