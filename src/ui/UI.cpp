#include "UI.h"

#include <string>
#include <iostream>

#include "utils.h"

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
    MenuItem listingsItem("Listings", {MenuItem("DBG")});
    MenuItem notificationsItem("Notifications", {}, [] {  });

    // Profile
    MenuItem profileItem("Profile", {
        MenuItem("Check balance", {
            MenuItem("Wallet", {}, [this] {walletBalanceLeaf();}),
            MenuItem("Bank", {}, [this] {bankBalanceLeaf();})
        }),
        MenuItem("My listings", {})
    });

    Menu menu({}, false);
    menu.items.push_back(listingsItem);
    menu.items.emplace_back(notificationsItem);
    menu.items.emplace_back(profileItem);

    MenuItem* listingsItemPtr = &menu.items[0];
    menu.items[0].action = [this, listingsItemPtr] { updateListingsItem(listingsItemPtr); };
    MenuItem* myListingsPtr = &menu.items[2].items[1];
    menu.items[2].items[1].action = [this, myListingsPtr] {updateMyListingsItem(myListingsPtr); };

    menu.run();
}
void UI::listingMenu(Listing listing) {
    Menu menu({}, true);

    menu.headerMessage = ""; // TODO: incomplete

    // TODO: BUY, BID OR NEG
    MenuItem buyItem("Buy" /* TODO: Add action to buy */);

    // TODO: only if owner
    MenuItem deleteItem("Delete");
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

    // TODO: call action to reg user
}
void UI::walletBalanceLeaf() {
    // TODO: get wallet from this.logic.logged_in_user.get_wallet().
    std::cout << "You have {bal}{cur} in wallet provided by {prov}." << std::endl;
    wait();
}
void UI::bankBalanceLeaf() {
    // TODO: get bank from this.logic.logged_in_user.get_bank().
    std::cout << "You have {bal}{cur} in {prov} bank." << std::endl;
    wait();
}

// Updates
void UI::updateListingsItem(MenuItem* listingsItem) {
    // TODO: call logic to get all listings and update the listingsItem
    listingsItem->items = {};
    // listingsItem->items.emplace_back(MenuItem("Listing 1", {}, []() {  }));
}
void UI::updateMyListingsItem(MenuItem* myListingsItem) {
    // TODO: call logic to get all my listings and update
    myListingsItem->items = {};
    // listingsItem->items.emplace_back(MenuItem("Listing 1", {}, []() {  }));
    myListingsItem->items.emplace_back(MenuItem("Create Listing", {}, []() {  }));
}
