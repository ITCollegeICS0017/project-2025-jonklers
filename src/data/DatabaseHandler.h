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
    DatabaseHandler() = default;
    DatabaseHandler(std::string user_fp, std::string listing_fp); // file paths
    void set_user_file(std::string new_path);
    void set_listing_file(std::string new_path);
    void set_archived_file(std::string new_path);

    void set_curr_usr(std::unique_ptr<User> up) { this->current_user = std::move(up); }
    void set_listings();

    User& get_curr() { return *this->current_user;}

    //Read
    std::unique_ptr<User> load_user(std::string user_id);
    std::vector<std::shared_ptr<Listing>> load_all_listings();
    std::unique_ptr<Listing> load_single_listing(std::string listing_id);

    //Write
    void update_usr(const User& u);
    void register_usr(const User& u);
    void delete_usr(const User& u);
    void add_listing(const std::unique_ptr<Listing> l);
    void archive_listing(std::string l_id);
    void delete_listing(std::string l_id);

private:
    std::unordered_map<std::string, std::shared_ptr<Listing>> all_listings;
    std::unique_ptr<User> current_user;
    std::string usr_filepath = "storage/users.json";
    std::string lst_filepath = "sorage/active_listings.json";
    std::string archive_filepath = "storage/archived_listings.json";
};