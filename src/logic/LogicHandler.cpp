#include "LogicHandler.h"
#include <memory>
#include <vector>

std::vector<std::shared_ptr<Listing>> LogicHandler::get_filtered(std::vector<std::shared_ptr<Listing>> to_filter, Category category){
    //TODO
    std::vector<std::shared_ptr<Listing>> v;
    return v;
}

std::vector<std::shared_ptr<Listing>> LogicHandler::get_sorted(std::vector<std::shared_ptr<Listing>> to_sort, std::string field){
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

bool LogicHandler::create_listing() {

    return true;
}