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

    User& get_current_user() { return db.get_curr();}


    std::vector<std::shared_ptr<Listing>> get_filtered(std::vector<std::shared_ptr<Listing>> to_filter, Category category);
    std::vector<std::shared_ptr<Listing>> get_sorted(std::vector<std::shared_ptr<Listing>> to_sort, std::string field); 
private:
    DatabaseHandler db;
};