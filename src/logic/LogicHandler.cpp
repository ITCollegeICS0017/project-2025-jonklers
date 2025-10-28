#include "LogicHandler.h"
#include <memory>
#include <vector>

std::unique_ptr<User> log_in(std::string u_id, std::string passwd){
    //TODO
    return std::make_unique<User>();
}

std::unique_ptr<User> register_user(std::string u_id, std::string psswd){
    //TODO
    return std::make_unique<User>();
}

std::vector<std::shared_ptr<Listing>> get_filtered(std::vector<std::shared_ptr<Listing>> to_filter, Category category){
    //TODO
    std::vector<std::shared_ptr<Listing>> v;
    return v;
}

std::vector<std::shared_ptr<Listing>> get_sorted(std::vector<std::shared_ptr<Listing>> to_sort, std::string field){
    //TODO
    std::vector<std::shared_ptr<Listing>> v;
    return v;
}

bool LogicHandler::log_in(std::string usrname, std::string password) {

    return true;
}

bool LogicHandler::register_user(std::string usrname, std::string password) {

    return true;
}