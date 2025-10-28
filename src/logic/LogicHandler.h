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
    LogicHandler();
    std::unique_ptr<User> log_in(std::string u_id, std::string passwd);
    void register_user(std::string u_id, std::string passwd);
    std::vector<std::shared_ptr<Listing>> get_filtered(std::vector<std::shared_ptr<Listing>> to_filter, Category category);
    std::vector<std::shared_ptr<Listing>> get_sorted(std::vector<std::shared_ptr<Listing>> to_sort, std::string field); 
private:
    DatabaseHandler db;
};