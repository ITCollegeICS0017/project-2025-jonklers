#pragma once
#include "../data/DatabaseHandler.h"
#include "../base/User.h"
#include "../base/Listing.h"
#include "../base/Auction.h"
#include "../base/Negotiation.h"
#include <future>
#include <memory>
#include <string>
#include <vector>

enum class Category;

class LogicHandler {
public:
    LogicHandler() = default;
    bool log_in(std::string u_id, std::string passwd);
    bool register_user(std::string u_id, std::string passwd);
    bool delete_user();
    bool create_listing(std::string type, std::string name, std::string description, Category category, double price);


    User& get_current_user() { return db.get_curr();}
    // buy bid neg
    bool conclude_sale(std::shared_ptr<Listing> l, std::string method); //method = "Wallet" or "BankAccount"
    bool place_bid(std::shared_ptr<Auction> l, std::string method, double amount);
    bool negotiate(std::shared_ptr<Negotiation> l, std::string method, double amount);
    //TODO:
    bool delete_listing(std::string id);
    std::vector<std::shared_ptr<Listing>> get_all_listings();
    std::vector<std::shared_ptr<Listing>> get_user_listings();
    std::vector<std::shared_ptr<Listing>> get_archived_listings();
    std::shared_ptr<Listing> get_single_listing(std::string id);

    std::vector<std::shared_ptr<Listing>> get_filtered(std::vector<std::shared_ptr<Listing>> to_filter, Category category);
    std::vector<std::shared_ptr<Listing>>& get_sorted(std::vector<std::shared_ptr<Listing>> to_sort, bool descending);
private:
    DatabaseHandler db;
};