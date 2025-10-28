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

std::vector<std::shared_ptr<Listing>> LogicHandler::get_sorted(bool descending){
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

std::vector<std::shared_ptr<Listing>> LogicHandler::get_all_listings() {
    std::vector<std::shared_ptr<Listing>> listings;
    for(const auto& [key, value] : db.get_map()) {
        listings.push_back(value);
    }
    return listings;
}

//CHECK FOR NULLPTR IN UI!!
std::shared_ptr<Listing> LogicHandler::get_single_listing(std::string id) {
    auto m = db.get_map();
    return (m.find(id) != m.end()) ? m[id] : nullptr;
}

bool LogicHandler::delete_listing(std::string id) {
    auto m = db.get_map();
    if(m.find(id) == m.end()) return false; //error? delete unexistent listing - catch it in ui and display something like "Already deleted" if this happens ever
    m.erase(id);
    try{
        db.update_listings_file();
    }catch(...){} //idk
    return true;
}

std::vector<std::shared_ptr<Listing>> LogicHandler::get_user_listings() {
    std::vector<std::shared_ptr<Listing>> listings;
    for(const auto& [key, value] : db.get_map()) {
        if(value->get_owner_id() == db.get_curr().get_id()) listings.push_back(value);
    }
    return listings;
}

bool LogicHandler::conclude_sale(std::shared_ptr<Listing> l, std::string method) {
    auto buyer = db.get_curr();
    auto seller = *db.load_user(l->get_owner_id());
    if(method == "Wallet"){
       buyer.update_wallet(true, l->get_price());
       seller.update_wallet(false, l->get_price());
    }else if(method == "BankAccount"){
       buyer.update_bank_account(true, l->get_price());
       seller.update_bank_account(false, l->get_price());
    }else{
        return false;
    }
    db.archive_listing(l->get_listing_id());
    return true;
}

bool LogicHandler::place_bid(std::shared_ptr<Auction> l, double amount) {
    try{
        l->set_price(amount);
        l->set_last_bidder(db.get_curr().get_id());
    }catch(...) {return false;}
    return true;
}

bool LogicHandler::negotiate(std::shared_ptr<Negotiation> l, double amount) {
    try{
        Offer o;
        o.sender_id = db.get_curr().get_id();
        o.neg_amount = amount;
        l->add_offer(o);
    }catch(...){return false;}
    return true;
}