#include "UI.h"

#include <string>
#include <iostream>

UI::UI() {}

// Menus
void UI::startupMenu() {
    std::vector<MenuItem> items = {
        MenuItem("Login", {}, [this] { loginLeaf(); }),
        MenuItem("Register", {}, [this] { registerLeaf(); }),
    };

    Menu menu(items);
    menu.run();
}
void UI::mainMenu() {
    MenuItem listingsItem("Listings", {MenuItem("wasd")});
    MenuItem* listingsItemPtr = &listingsItem;
    listingsItem.action = [this, listingsItemPtr] { updateListingsItem(listingsItemPtr); };
    MenuItem notificationsItem("Notifications", {}, [] {  });
    MenuItem profileItem("Profile", {}, [] {  });

    updateListingsItem(listingsItemPtr);
    Menu menu({}, true, {0, 1});
    menu.items.emplace_back(listingsItem);
    menu.items.emplace_back(notificationsItem);
    menu.items.emplace_back(profileItem);

    menu.run();
}

// Leafs
void UI::loginLeaf() {
    std::string username;
    std::string password;

    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    // TODO: call action to log in user

    mainMenu();
}
void UI::registerLeaf() {
    std::string username;
    std::string password;

    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    // TODO: call action to register user
}

// Updates
void UI::updateListingsItem(MenuItem* listingsItem) {
    // listingsItem->items = {
    //     MenuItem("Listing 1", {}, []() {  }),
    //     MenuItem("Listing 2", {}, []() {  }),
    //     MenuItem("Listing 3", {}, []() {  }),
    // };
}