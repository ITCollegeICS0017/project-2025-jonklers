#pragma once

#include <mutex>
#include <unordered_map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <thread>
#include <atomic>


class User;
class Listing;

class DatabaseHandler {
public:
    DatabaseHandler();
    ~DatabaseHandler() = default;
    DatabaseHandler(std::string user_fp, std::string listing_fp); // file paths
    void set_user_file(std::string new_path);
    void set_listing_file(std::string new_path);
    void set_archived_file(std::string new_path);

    void set_curr_usr(std::unique_ptr<User> up) { this->current_user = std::move(up); }

    User& get_curr() { return *this->current_user;}
    std::unordered_map<std::string, std::shared_ptr<Listing>>& get_map() { return this->all_listings;}

    //Read
    std::unique_ptr<User> load_user(std::string user_id);
    void load_all_listings();
    //std::unique_ptr<Listing> load_single_listing(std::string listing_id);

    void create_files();

    //Write
    void update_usr(const User& u);
    void register_usr(const User& u);
    void delete_usr(const User& u);
    void add_listing(std::shared_ptr<Listing> l);
    void update_listings_file();
    void archive_listing(std::string l_id);
    void append_archive(std::shared_ptr<Listing> l);
    std::vector<std::shared_ptr<Listing>> get_archived();

private:
    std::unordered_map<std::string, std::shared_ptr<Listing>> all_listings;
    std::unique_ptr<User> current_user;

    std::mutex mut;
    std::atomic<bool> check_expiry;
    std::thread worker;

    void loop();

    std::string usr_filepath = "storage/users.json";
    std::string lst_filepath = "storage/active_listings.json";
    std::string archive_filepath = "storage/archived_listings.json";
};