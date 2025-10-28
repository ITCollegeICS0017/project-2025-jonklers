#include "UI.h"

#include <string>
#include <iostream>

UI::UI() {}

// Menus
void UI::startupMenu() {
    std::vector<MenuItem> items = {
        MenuItem("Login", [this]() { loginLeaf(); }),
        MenuItem("Register", [this]() { registerLeaf(); }),
    };

    Menu menu(items);
    menu.run();
}
void UI::mainMenu() {
    MenuItem listingsItem = MenuItem("Listings", [this, &listingsItem]() { updateListingsItem(listingsItem); });

    std::vector<MenuItem> items = {
        listingsItem,
        MenuItem("Notifications", [this]() {  }),
        MenuItem("Profile", [this]() {  }),
    };

    Menu menu(items);
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
void UI::updateListingsItem(MenuItem& listingsItem) {
    listingsItem.items = {
        MenuItem("Listing 1", [this]() {  }),
        MenuItem("Listing 2", [this]() {  }),
        MenuItem("Listing 3", [this]() {  }),
    };
}