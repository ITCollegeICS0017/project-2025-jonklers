#pragma once

#include "nlohmann/json.hpp"
#include <fstream>
#include <memory>
#include <string>
#include <vector>

using json = nlohmann::json;

class User;
class Listing;

class DatabaseHandler {
public:
    DatabaseHandler() = default;
    DatabaseHandler(std::string dir, std::string users_file, std::string listings_file);
    void set_dir(std::string new_dir);
    void set_user_file(std::string set);
    void set_listing_file();

    std::unique_ptr<User> get_user(std::string user_id);
    std::unique_ptr<Listing> get_single_listing(std::string listing_id);
    std::vector<std::shared_ptr<Listing>> get_all_listings();

private:
    json file;
    std::string dir = "storage";
    std::string users_file = "users.json";
    std::string listings_file = "listings.json";
};