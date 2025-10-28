#include "UI.h"

#include <string>
#include <iostream>

#include "utils.h"

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
        MenuItem("My listings", {}),
        MenuItem("My notifications", {})
    });

    Menu menu({}, false);
    menu.items.push_back(listingsItem);
    menu.items.emplace_back(notificationsItem);
    menu.items.emplace_back(profileItem);

    MenuItem* listingsItemPtr = &menu.items[0];
    menu.items[0].action = [this, listingsItemPtr] { updateListingsItem(listingsItemPtr); };
    MenuItem* myListingsPtr = &menu.items[2].items[1];
    menu.items[2].items[1].action = [this, myListingsPtr] {updateMyListingsItem(myListingsPtr); };
    MenuItem* myNotisPtr = &menu.items[2].items[2];
    menu.items[2].items[2].action = [this, myNotisPtr] { updateMyNotifications(myNotisPtr); };

    menu.run();
}
void UI::listingMenu(std::shared_ptr<Listing> listing) {
    Menu menu({}, true);

    menu.exitLabel = "Back";
    menu.headerMessage = "Product: " + listing->get_product().name + "\n";
    menu.headerMessage += "Description: " + listing->get_product().description + "\n";
    menu.headerMessage += "Category: " + enumToStrCategory(listing->get_product().category) + "\n";
    menu.headerMessage += "Price: " + std::to_string(listing->get_price()) + "\n";
    menu.headerMessage += "Expires: " + std::to_string(listing->get_expiry()) + "\n";

    if (listing->get_owner_id() == logic.get_current_user().get_id()) {
        MenuItem deleteItem("Delete", {}, [this, listing] { logic.delete_listing(listing->get_listing_id()); });
    }
    else {
        // TODO: or any other order type
        MenuItem buyItem("Buy" /* TODO: Add action to buy */);
    }

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

    bool isValid = logic.log_in(username, password);

    if (isValid)
        mainMenu();
    else {
        std::cout << "Invalid username or password.\n";
        wait();
    }
}
void UI::registerLeaf() {
    std::string username;
    std::string password;

    std::cout << "Username: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);

    bool isValid = logic.register_user(username, password);

    if (isValid) {
        mainMenu();
    }
    else {
        std::cout << "Invalid username or password.\n";
        wait();
    }
}
void UI::walletBalanceLeaf() {
    Wallet wallet = logic.get_current_user().get_wallet();
    std::cout << "You have " << wallet.balance << enumToStrCrypto(wallet.curr) << " in wallet provided by " << wallet.provider << "." << std::endl;
    wait();
}
void UI::bankBalanceLeaf() {
    BankAccount bank = logic.get_current_user().get_bank_account();
    std::cout << "You have " << bank.balance << enumToStrFiat(bank.curr) << " in wallet provided by " << bank.provider << "." << std::endl;
    wait();
}

// Updates
void UI::updateListingsItem(MenuItem* listingsItem) { // TODO: might need opti
    listingsItem->items = {};

    for (auto listing : logic.get_all_listings()) {
        MenuItem listingItem(listing->get_product().name, {}, [this, listing] { listingMenu(logic.get_single_listing(listing->get_listing_id())); });
        listingsItem->items.push_back(listingItem);
    }
}
void UI::updateMyListingsItem(MenuItem* myListingsItem) { // TODO: might need opti
    myListingsItem->items = {};

    for (auto listing : logic.get_user_listings()) {
        MenuItem listingItem(listing->get_product().name, {}, [this, listing] { listingMenu(logic.get_single_listing(listing->get_listing_id())); });
        myListingsItem->items.push_back(listingItem);
    }
}
void UI::updateMyNotifications(MenuItem* myNotifications) { // TODO: might need opti
    myNotifications->items = {};

    for (auto notification : logic.get_current_user().get_messages()) {
        MenuItem notiItem(notification.body, {}, [this, notification] { listingMenu(logic.get_single_listing(notification.listing_id)); });
        myNotifications->items.emplace_back(notiItem);
    }
}
