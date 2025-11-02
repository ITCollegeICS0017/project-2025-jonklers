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
    password = getPassword();

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
    password = getPassword();

    bool isValid = logic.register_user(username, password);

    if (isValid) {
        mainMenu();
    }
    else {
        std::cout << "Invalid username or password.\n";
        wait();
    }
}
void UI::buyLeaf(std::shared_ptr<Listing> listing, std::shared_ptr<Menu> menu) {
    bool res = logic.conclude_sale(listing);

    if (res) {
        std::cout << "Successfully concluded sale.\n";
    }
    else
        std::cout << "Error concluding sale.\n";
    wait();
    menu->keys = {0};
}
void UI::bidLeaf(std::shared_ptr<Listing> listing, std::shared_ptr<Menu> menu) {
    double price;
    std::cout << "Bid price: ";
    std::cin.precision(2);
    std::cin >> price;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bool res = logic.place_bid(std::dynamic_pointer_cast<Auction>(listing), price);

    if (res) {
        std::cout << "Successfully bid on listing.\n";
    }
    else
        std::cout << "Error bidding on listing.\n";
    wait();
    menu->keys = {0};
}
void UI::negotiateLeaf(std::shared_ptr<Listing> listing, std::shared_ptr<Menu> menu) {
    double price;
    std::cout << "Offer price: ";
    std::cin.precision(2);
    std::cin >> price;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    bool res = logic.negotiate(std::dynamic_pointer_cast<Negotiation>(listing), price);

    if (res) {
        std::cout << "Successfully made offer on listing.\n";
    }
    else
        std::cout << "Error making offer for listing.\n";
    wait();
    menu->keys = {0};
}




void UI::mainMenu() {
    auto listingsItem = std::make_shared<MenuItem>("Listings", [] {});
    auto messagesItem = std::make_shared<MenuItem>("Messages", [] {});
    auto profileItem = std::make_shared<MenuItem>("Profile", [] {});

    auto myBalance = std::make_shared<MenuItem>("My Balance", [] {});
    auto myListingsItem = std::make_shared<MenuItem>("My Listings", [] {});
    auto myHistory = std::make_shared<MenuItem>("My History", [] {});

    auto walletItem = std::make_shared<MenuItem>("My Wallet", [this] {walletLeaf();});
    auto bankItem = std::make_shared<MenuItem>("My Bank", [this] {bankLeaf();});
    auto gcItem = std::make_shared<MenuItem>("GorillaCoin Balance", [this] {});

    myBalance->items = {gcItem, walletItem, bankItem};

    profileItem->items = {myBalance, myListingsItem, myHistory};

    auto menu = std::make_shared<Menu>(std::make_shared<MenuItem>("mainMenu", std::vector<std::shared_ptr<MenuItem>> {
        listingsItem,
        messagesItem,
        profileItem,
    }, [] {}));

    gcItem->action = [this, gcItem, menu] {addGC(gcItem, menu);};
    gcItem->action();

    myListingsItem->action = [this, menu, myListingsItem] {addListings(menu, myListingsItem, myListingsItem, logic.get_user_listings());};
    myListingsItem->action();

    myHistory->action = [this, menu, myHistory] {addListings(menu, myHistory, myHistory, logic.get_archived_listings(), false);};
    myHistory->action();

    listingsItem->action = [this, menu, listingsItem] {addListings(menu, listingsItem, listingsItem, logic.get_all_listings());};
    listingsItem->action();

    messagesItem->action = [this, menu, messagesItem] {addMessages(menu, messagesItem, messagesItem, logic.get_current_user().get_messages());};
    messagesItem->action();

    menu->run();
}

void UI::convertLeaf(std::string method, std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> gcItem) {
    double amount;
    std::cout << "Enter amount: ";
    std::cin >> amount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto w = logic.get_current_user().get_wallet();
    auto b = logic.get_current_user().get_bank_account();

    if (
        (method == "Wallet" && amount > w.balance) ||
        (method == "Bank" && amount > b.balance)) {
        std::cout << "Not enough on respective balance." << std::endl;
        wait();
        return;
    }

    double amount_to;
    if (method == "Wallet") {
        amount_to = logic.to_gorilla_coin(w.curr, amount);
    }
    else if (method == "Bank") {
        amount_to = logic.to_gorilla_coin(b.curr, amount);
    }

    std::string confirm;
    std::cout << "Confirm conversion to " + std::to_string(amount_to) + "GC [y]/n: ";
    std::getline(std::cin, confirm);

    if (confirm == "n") {
        std::cout << "Canceled conversion." << std::endl;
        wait();
        return;
    }

    if (method == "Wallet") {
        if (!logic.recharge_balance("Wallet", amount)) {
            std::cout << "Failed" << std::endl;
            wait();
            return;
        }
    }
    else if (method == "Bank") {
        if (!logic.recharge_balance("BankAccount", amount)) {
            std::cout << "Failed" << std::endl;
            wait();
            return;
        }
    }

    std::cout << "Successfully converted." << std::endl;
    wait();
    gcItem->action();
}


void UI::addGC(std::shared_ptr<MenuItem> parent, std::shared_ptr<Menu> menu) {
    parent->header = "Current GorillaCoin balance: " + std::to_string(logic.get_current_user().get_balance()) + "\n";
    parent->items.clear();

    auto wCur = logic.get_current_user().get_wallet().curr;
    auto bCur = logic.get_current_user().get_bank_account().curr;
    auto rechargeWalletItem = std::make_shared<MenuItem>("Recharge with Wallet  [1" + CurrencyToString(wCur) + "=" + std::to_string(logic.to_gorilla_coin(wCur, 1)) + "GC]", [this, menu, parent] {convertLeaf("Wallet", menu, parent);});
    auto rechargeBankItem = std::make_shared<MenuItem>("Recharge with Bank    [1" + CurrencyToString(bCur) + "=" + std::to_string(logic.to_gorilla_coin(bCur, 1)) + "GC]", [this, menu, parent] {convertLeaf("Bank", menu, parent);});

    parent->items.push_back(rechargeWalletItem);
    parent->items.push_back(rechargeBankItem);
}

void UI::walletLeaf() {
    Wallet wallet = logic.get_current_user().get_wallet();
    std::cout << "You have " << wallet.balance << CurrencyToString(wallet.curr) << " in wallet provided by " << wallet.provider << "." << std::endl;
    wait();
}
void UI::bankLeaf() {
    BankAccount bank = logic.get_current_user().get_bank_account();
    std::cout << "You have " << bank.balance << CurrencyToString(bank.curr) << " in wallet provided by " << bank.provider << "." << std::endl;
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
        logic.create_listing(type, name, description, category, price);
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


void UI::addListings(std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> destination, std::shared_ptr<MenuItem> parent, std::vector<std::shared_ptr<Listing>> listings, bool can_create) {
    parent->items.clear();

    if (can_create)
        addCreateListing(menu, destination, parent);

    auto sortAscItem = std::make_shared<MenuItem>("Sort by Price (Asc)", [this, menu, destination, parent, listings] {
        addListings(menu, destination, parent, logic.get_sorted(listings, false));
    });
    auto sortDescItem = std::make_shared<MenuItem>("Sort by Price (Desc)", [this, menu, destination, parent, listings] {
        addListings(menu, destination, parent, logic.get_sorted(listings, true));
    });
    auto filterItem = std::make_shared<MenuItem>("Filter by Category", [] {});
    for (auto category : {Category::ELECTRONICS, Category::BOOKS, Category::FASHION, Category::GARDEN, Category::HOME}) {
        auto categoryItem = std::make_shared<MenuItem>(enumToStrCategory(category), [this, menu, destination, parent, listings, category, filterItem] {
            menu->gotoItem(filterItem);addListings(menu, destination, parent, logic.get_filtered(listings, category));
        });
        filterItem->items.push_back(categoryItem);
    }
    parent->items.push_back(sortAscItem);
    parent->items.push_back(sortDescItem);
    parent->items.push_back(filterItem);

    for (auto listing : listings) {
        auto listingItem = std::make_shared<MenuItem>(listing->get_product().name, [] {});
        addListing(menu, destination, listingItem, listing, can_create);
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
void UI::addListing(std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> destination, std::shared_ptr<MenuItem> parent, std::shared_ptr<Listing> listing, bool can_act) {
    parent->items.clear();

    parent->header = "Name: " + listing->get_product().name + "\n";
    parent->header += "Description: " + listing->get_product().description + "\n";
    parent->header += "Category: " + enumToStrCategory(listing->get_product().category) + "\n";
    parent->header += "Price: " + std::to_string(listing->get_price()) + "\n";
    parent->header += "Expires: " + std::to_string(listing->get_expiry()) + "\n";

    // Skip all subitems
    if (!can_act) {
        parent->items.push_back(std::make_shared<MenuItem>("This listing is expired", [] {}));
        return;
    }

    if (listing->get_owner_id() != logic.get_current_user().get_id()) {
        auto buyItem = std::make_shared<MenuItem>("Buy", [this, listing, menu] {buyLeaf(listing, menu);});
        auto bidItem = std::make_shared<MenuItem>("Bid", [this, listing, menu] {bidLeaf(listing, menu);});
        auto negotiateItem = std::make_shared<MenuItem>("Negotiate", [this, listing, menu] {negotiateLeaf(listing, menu);});

        if (listing->type() == "Listing") {
            parent->items.push_back(buyItem);
        } else if (listing->type() == "Auction") {
            parent->items.push_back(bidItem);
        } else if (listing->type() == "Negotiation") {
            parent->items.push_back(buyItem);
            parent->items.push_back(negotiateItem);
        }
    }
    else {
        auto deleteItem = std::make_shared<MenuItem>("Delete", [this, listing, menu, destination] {logic.delete_listing(listing->get_listing_id());menu->gotoItem(destination);});
        parent->items.push_back(deleteItem);

        if (listing->type() == "Negotiation") {
            auto listneg = std::dynamic_pointer_cast<Negotiation>(listing);

            for (auto offer : listneg->get_offers()) {
                auto offerItem = std::make_shared<MenuItem>("Offer ["+std::to_string(offer.neg_amount)+"GC]", [] {});
                offerItem->header = "Item: " + listing->get_product().name + "\nOffer for " + std::to_string(offer.neg_amount) + "GC, made by: " + offer.sender_id + "\n";
                offerItem->items.push_back(std::make_shared<MenuItem>("Accept", [this, listneg, &offer, menu] {respondLeaf(true, listneg, offer, menu);}));
                offerItem->items.push_back(std::make_shared<MenuItem>("Reject", [this, listneg, &offer, menu] {respondLeaf(false, listneg, offer, menu);}));
                parent->items.push_back(offerItem);
            }
        }
    }
}

void UI::respondLeaf(bool accept, std::shared_ptr<Negotiation> listneg, Offer &offer, std::shared_ptr<Menu> menu) {
    auto res = logic.respond_offer(accept, listneg, offer);

    if (res) {
        std::cout << "Response succeeded." << std::endl;
        menu->keys = {0};
    } else
        std::cout << "Response failed." << std::endl;

    wait();
}
