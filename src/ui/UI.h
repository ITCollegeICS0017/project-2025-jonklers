#pragma once

#include "../base/Listing.h"
#include "Menu.h"
#include <vector>

#include "../logic/LogicHandler.h"

class UI {
    public:
    LogicHandler logic;

    UI() = default;
    
    void loginLeaf();
    void registerLeaf();
    void walletBalanceLeaf();
    void bankBalanceLeaf();

    void startupMenu(); 
    void mainMenu();
    void listingMenu(std::shared_ptr<Listing> listing);

    void updateListingsItem(MenuItem* listingsItem);
    void updateMyListingsItem(MenuItem* myListingsItem);
    void updateMyNotifications(MenuItem* myNotifications);
};
