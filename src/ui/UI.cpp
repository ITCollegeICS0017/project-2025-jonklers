#include "UI.h"

#include <iostream>
#include "utils.h"

void UI::startupMenu() {
    Menu menu(std::make_shared<MenuItem>("startupMenu", std::vector<std::shared_ptr<MenuItem>> {
        std::make_shared<MenuItem>("Login", [this] {loginLeaf();}),
        std::make_shared<MenuItem>("Register", [this] {registerLeaf();}),
    }, [] {}));

    menu.run();
}
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




void UI::mainMenu() {
    auto listingsItem = std::make_shared<MenuItem>("Listings", [] {});
    auto messagesItem = std::make_shared<MenuItem>("Messages", [] {});
    auto profileItem = std::make_shared<MenuItem>("Profile", [] {});

    auto walletItem = std::make_shared<MenuItem>("My Wallet", [this] {walletLeaf();});
    auto bankItem = std::make_shared<MenuItem>("My Bank", [this] {bankLeaf();});
    auto myListingsItem = std::make_shared<MenuItem>("My Listings", [] {});

    profileItem->items = {walletItem, bankItem, myListingsItem};

    auto menu = std::make_shared<Menu>(std::make_shared<MenuItem>("mainMenu", std::vector<std::shared_ptr<MenuItem>> {
        listingsItem,
        messagesItem,
        profileItem,
    }, [] {}));

    myListingsItem->action = [this, menu, myListingsItem] {addListings(menu, myListingsItem, myListingsItem, logic.get_all_listings());};
    myListingsItem->action();

    listingsItem->action = [this, menu, listingsItem] {addListings(menu, listingsItem, listingsItem, logic.get_all_listings());};
    listingsItem->action();

    messagesItem->action = [this, menu, messagesItem] {addMessages(menu, messagesItem, messagesItem, logic.get_current_user().get_messages());};
    messagesItem->action();

    menu->run();
}

void UI::walletLeaf() {
    Wallet wallet = logic.get_current_user().get_wallet();
    std::cout << "You have " << wallet.balance << enumToStrCrypto(wallet.curr) << " in wallet provided by " << wallet.provider << "." << std::endl;
    wait();
}
void UI::bankLeaf() {
    BankAccount bank = logic.get_current_user().get_bank_account();
    std::cout << "You have " << bank.balance << enumToStrFiat(bank.curr) << " in wallet provided by " << bank.provider << "." << std::endl;
    wait();
}
void UI::createListingLeaf(std::string type, Category category) {
    std::string name;
    std::string description;
    float price;

    std::cout << "Name: ";
    std::getline(std::cin, name);
    std::cout << "Description: ";
    std::getline(std::cin, description);
    std::cout << "Price: ";
    std::cin.precision(2);
    std::cin >> price;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (price > 0 && !name.empty()) {
        // logic.create_listing(type, name, description, category, price); // TODO: uncomment on create_listing signature change
        std::cout << "Successfully created listing." << std::endl;
    } else {
        std::cout << "Error while createing listing.\nMake sure to have non-empty name, price > 0\n";
    }

    wait();
}

void UI::addCreateListing(std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> destination, std::shared_ptr<MenuItem> parent) {
    auto createListingItem = std::make_shared<MenuItem>("Create listing", [] {});
    createListingItem->header = "Select sale type\n";

    for (auto type : {"Listing", "Auction", "Negotiation"}) {
        auto typeItem = std::make_shared<MenuItem>(type, [] {});
        typeItem->header = "Select sale category\n";
        for (auto category : {Category::ELECTRONICS, Category::BOOKS, Category::FASHION, Category::GARDEN, Category::HOME}) {
            auto categoryItem = std::make_shared<MenuItem>(enumToStrCategory(category), [this, type, category, menu, destination] {createListingLeaf(type, category);menu->gotoItem(destination);});
            typeItem->items.push_back(categoryItem);
        }
        createListingItem->items.push_back(typeItem);
    }

    parent->items.push_back(createListingItem);
}


void UI::addListings(std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> destination, std::shared_ptr<MenuItem> parent, std::vector<std::shared_ptr<Listing>> listings) {
    parent->items.clear();

    addCreateListing(menu, destination, parent);

    // TODO: add filter and sort

    for (auto listing : listings) {
        auto listingItem = std::make_shared<MenuItem>(listing->get_product().name, [] {});
        addListing(menu, destination, listingItem, listing);
        parent->items.push_back(listingItem);
    }
}
void UI::addMessages(std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> destination, std::shared_ptr<MenuItem> parent, std::vector<Message> messages) {
    parent->items.clear();

    for (auto message : messages) {
        auto messageItem = std::make_shared<MenuItem>(message.body, [] {});
        addListing(menu, destination, messageItem, logic.get_single_listing(message.listing_id));
        if (!message.listing_id.empty()) {}
        parent->items.push_back(messageItem);
    }
}
void UI::addListing(std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> destination, std::shared_ptr<MenuItem> parent, std::shared_ptr<Listing> listing) {
    parent->items.clear();

    parent->header = "Name: " + listing->get_product().name + "\n";
    parent->header = "Description: " + listing->get_product().description + "\n";
    parent->header = "Category: " + enumToStrCategory(listing->get_product().category) + "\n";
    parent->header = "Price: " + std::to_string(listing->get_price()) + "\n";
    parent->header = "Expires: " + std::to_string(listing->get_expiry()) + "\n";

    if (listing->get_owner_id() != logic.get_current_user().get_id()) {
        // TODO: other buying options
        auto buyItem = std::make_shared<MenuItem>("Buy", [] {});
        parent->items.push_back(buyItem);
    }
    else {
        auto deleteItem = std::make_shared<MenuItem>("Delete", [this, listing, menu, destination] {logic.delete_listing(listing->get_listing_id());menu->gotoItem(destination);});
        parent->items.push_back(deleteItem);
    }
}
