#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <utility>
#include <vector>


class User;
class Listing;

class DatabaseHandler {
public:
    DatabaseHandler(std::string user_fp, std::string listing_fp); // file paths
    void set_user_file(std::string new_path);
    void set_listing_file(std::string new_path);

    std::unique_ptr<User> load_user(std::string user_id);
    void set_curr_usr(std::unique_ptr<User> up) { this->current_user = std::move(up); }
    std::unique_ptr<Listing> load_single_listing(std::string listing_id);
    std::vector<std::shared_ptr<Listing>> load_all_listings();

private:
    std::unordered_map<std::string, std::shared_ptr<Listing>> all_listings;
    std::unique_ptr<User> current_user;
    std::string usr_filepath;
    std::string lst_filepath;
};