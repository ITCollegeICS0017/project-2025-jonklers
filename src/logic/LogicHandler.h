#pragma once
#include "../data/DatabaseHandler.h"
#include "../base/User.h"
#include "../base/Listing.h"
#include <memory>
#include <string>
#include <vector>

enum class Category;

class LogicHandler {
public:
    LogicHandler() = default;
    bool log_in(std::string u_id, std::string passwd);
    bool register_user(std::string u_id, std::string passwd);
    bool create_listing(std::string type, std::string name, std::string description, std::string category, double price);


    User& get_current_user() { return db.get_curr();}
    //delete, buy bid neg, get all listings, get curr user listings, get listing by id

    std::vector<std::shared_ptr<Listing>> get_filtered(Category category);
    std::vector<std::shared_ptr<Listing>> get_sorted(bool descending); 
private:
    DatabaseHandler db;
};