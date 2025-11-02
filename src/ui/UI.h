#pragma once

#include "../base/Listing.h"
#include "Menu.h"
#include <vector>

#include "../logic/LogicHandler.h"

class UI {
    public:
    LogicHandler logic;

    UI() = default;

    void startupMenu();
    void loginLeaf();
    void registerLeaf();

    void mainMenu();
    void walletLeaf();
    void bankLeaf();
    void createListingLeaf(std::string type, Category category);
    void buyLeaf(std::string method, std::shared_ptr<Listing> listing, std::shared_ptr<Menu> menu);
    void bidLeaf(std::string method, std::shared_ptr<Listing> listing, std::shared_ptr<Menu> menu);
    void negotiateLeaf(std::string method, std::shared_ptr<Listing> listing, std::shared_ptr<Menu> menu);

    void addListings(std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> destination, std::shared_ptr<MenuItem> parent, std::vector<std::shared_ptr<Listing>> listings, bool can_create = true);
    void addMessages(std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> destination, std::shared_ptr<MenuItem> parent, std::vector<Message> messages);
    void addListing(std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> destination, std::shared_ptr<MenuItem> parent, std::shared_ptr<Listing> listing, bool can_act = true);
    void addCreateListing(std::shared_ptr<Menu> menu, std::shared_ptr<MenuItem> destination, std::shared_ptr<MenuItem> parent);
};
