#pragma once

#include "../base/Listing.h"
#include "Menu.h"
#include <vector>

class UI {
    public:
    // TODO: reference to Logic object

    UI();
    
    void loginLeaf();
    void registerLeaf();
    void walletBalanceLeaf();
    void bankBalanceLeaf();

    void startupMenu(); 
    void mainMenu();
    void profileMenu(/* TODO: reference to notifications */);
    void notificationsMenu(/* TODO: reference to notifications */);
    void listingMenu(Listing listing);

    void updateListingsItem(MenuItem* listingsItem);
    void updateMyListingsItem(MenuItem* myListingsItem);
};