#pragma once
#include<memory>
#include<string>
#include<vector>

class User;
class Listing;

class DatabaseHandler {
public:
    DatabaseHandler();
    void set_dir();
    void set_user_file();
    void set_listing_file();

    std::unique_ptr<User> get_user(std::string user_id);
    std::vector<std::shared_ptr<Listing>> get_listings();

private:
    std::string dir;
    std::string user_file;
    std::string listing_file;
};