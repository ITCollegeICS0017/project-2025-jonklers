#include "LogicHandler.h"
#include "../base/Auction.h"
#include "../base/Negotiation.h"
#include <memory>
#include <vector>

std::vector<std::shared_ptr<Listing>> LogicHandler::get_filtered(Category category){
    //TODO
    std::vector<std::shared_ptr<Listing>> v;
    return v;
}

std::vector<std::shared_ptr<Listing>> LogicHandler::get_sorted(std::string field){
    //TODO
    std::vector<std::shared_ptr<Listing>> v;
    return v;
}

bool LogicHandler::log_in(std::string usrname, std::string password) {
    try{
        std::unique_ptr<User> u = db.load_user(usrname);
        if(!(u->get_password() == hash_password(password))) return false;
        db.set_curr_usr(std::move(u));
        return true;
    }catch(...) {return false;}
}

bool LogicHandler::register_user(std::string usrname, std::string password) {
    try{
        std::unique_ptr<User> u = std::make_unique<User>(usrname, password);
        db.register_usr(*u);
        db.set_curr_usr(std::move(u));
        return true;
    }catch(...) {return false;}
}

bool LogicHandler::create_listing(std::string type, std::string name, std::string description, std::string category, double price){
    if(type == "Listing") {
        db.add_listing(std::make_unique<Listing>()); 
    }else if(type == "Auction") {
        db.add_listing(std::make_unique<Auction>());
    }else if(type == "Negotiation") {
        db.add_listing(std::make_unique<Negotiation>());
    }else {
        return false;
    }
    return true;
}