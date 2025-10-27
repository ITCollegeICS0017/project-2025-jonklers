#pragma once

class UI {
    public:
    // TODO: reference to Logic object
    void loginLeaf();
    void registerLeaf();
    
    void startupMenu();
    void mainMenu();
    void profileMenu(/* TODO: reference to notifications */);
    void notificationsMenu(/* TODO: reference to notifications */);
    void listingsMenu(/* TODO: reference to listings */);
    void listingMenu(/* TODO: reference to listing */);
};