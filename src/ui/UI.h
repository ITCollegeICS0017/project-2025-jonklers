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
    
    void startupMenu(); 
    void mainMenu();
    void profileMenu(/* TODO: reference to notifications */);
    void notificationsMenu(/* TODO: reference to notifications */);
    void listingsMenu(std::vector<Listing> listings);
    void listingMenu(Listing listing);

    void updateListingsItem(MenuItem& listingsItem);
};