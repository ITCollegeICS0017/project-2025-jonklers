#include "LogicHandler.h"
#include "../base/User.h"
#include "../base/Auction.h"
#include "../base/Negotiation.h"
#include <memory>
#include <vector>

std::vector<std::shared_ptr<Listing>> LogicHandler::get_filtered(std::vector<std::shared_ptr<Listing>> to_filter,Category category){
    std::vector<std::shared_ptr<Listing>> filtered_v;
    for(auto l : to_filter) {
        if(l->get_product().category == category) filtered_v.push_back(l);
    }
    return filtered_v;
}

std::vector<std::shared_ptr<Listing>>& LogicHandler::get_sorted(std::vector<std::shared_ptr<Listing>>    to_sort, bool cheapest_first){
    std::sort(to_sort.begin(), to_sort.end(), [cheapest_first](const auto& a, const auto& b) { return (cheapest_first) ? (a->get_price() < b->get_price()) : (a->get_price() > b->get_price());});
    return to_sort;
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

bool LogicHandler::create_listing(std::string type, std::string name, std::string description, Category category, double price){
    Product p = {name, description, category};
    std::string o_id = db.get_curr().get_id();
    if(type == "Listing") {
        db.add_listing(std::make_unique<Listing>(price, o_id, p)); 
    }else if(type == "Auction") {
        db.add_listing(std::make_unique<Auction>(price, o_id, p));
    }else if(type == "Negotiation") {
        db.add_listing(std::make_unique<Negotiation>(price, o_id, p));
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

std::vector<std::shared_ptr<Listing>> LogicHandler::get_archived_listings() {
    std::vector<std::shared_ptr<Listing>> listings = db.get_own_archived();
    return listings;
}

std::shared_ptr<Listing> LogicHandler::get_single_listing(std::string id) {
    auto& m = db.get_map();
    return  m[id];
}

bool LogicHandler::delete_listing(std::string id) {
    auto& m = db.get_map();
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
    auto& buyer = db.get_curr();
    auto seller = *db.load_user(l->get_owner_id());
    if(buyer.get_balance() < l->get_price()) return false;
    buyer.update_balance(true, l->get_price());
    seller.update_balance(false, l->get_price());
    db.update_usr(buyer);
    db.update_usr(seller);
    db.archive_listing(l->get_listing_id());
    return true;
}

//TODO:: Make messages
bool LogicHandler::place_bid(std::shared_ptr<Auction> l, double amount) {
    auto& u = get_current_user();
    if(amount < l->get_price() || u.get_balance() < amount) return false;
    u.move_reserved(true, amount);

    //Unreserve previous bidder's money
    auto last_u = *db.load_user(l->get_last_bidder());
    last_u.move_reserved(false, l->get_price());
    db.update_usr(last_u);

    l->set_price(amount);
    l->set_last_bidder(u.get_id());
    return true;
}

bool LogicHandler::negotiate(std::shared_ptr<Negotiation> l, double amount) {
    auto& u = get_current_user();
    if(amount < 0 || u.get_balance() < amount) return false;
    Offer o;
    o.sender_id = u.get_id();
    o.neg_amount = amount;
    u.move_reserved(true, amount);
    l->add_offer(o);
    return true;
}

bool LogicHandler::delete_user() {
    try{
        auto usr_listings = get_user_listings();
        for(auto l : usr_listings) {
            delete_listing(l->get_listing_id());
        }
        db.delete_usr(db.get_curr());
    }catch(...) {return false;}
    return true;
}

bool LogicHandler::recharge_balance(std::string method, double amount) {
    auto& u = get_current_user();
    if(method == "Wallet") {
        u.update_wallet(true, amount);
        u.update_balance(false, to_gorilla_coin(u.get_wallet().curr, amount));
        return true;
    }else if(method == "BankAccount") {
        u.update_bank_account(true, amount);
        u.update_balance(false, to_gorilla_coin(u.get_bank_account().curr, amount));
        return true;
    }else return false;
}