#include "LogicHandler.h"
#include "../base/User.h"
#include "../base/Auction.h"
#include "../base/Negotiation.h"
#include <memory>
#include <vector>
#include <iomanip>
#include <sstream>

std::vector<std::shared_ptr<Listing>> LogicHandler::get_filtered(std::vector<std::shared_ptr<Listing>> to_filter,Category category){
    std::vector<std::shared_ptr<Listing>> filtered_v;
    for(auto l : to_filter) {
        if(l->get_product().category == category) filtered_v.push_back(l);
    }
    return filtered_v;
}

std::vector<std::shared_ptr<Listing>> LogicHandler::get_sorted(std::vector<std::shared_ptr<Listing>>    to_sort, bool cheapest_first){
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
    check_expiry(listings);
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
    check_expiry(listings);
    return listings;
}


bool LogicHandler::conclude_sale(std::shared_ptr<Listing> l) {
    std::vector<std::shared_ptr<Listing>> v;
    v.push_back(l);
    auto& buyer = db.get_curr();
    auto seller = *db.load_user(l->get_owner_id());
    if(buyer.get_balance() < l->get_price() || check_expiry(v)) return false;
    buyer.update_balance(true, l->get_price());
    seller.update_balance(false, l->get_price());
    db.update_usr(buyer);
    db.update_usr(seller);
    create_message(l, seller, MessageEvent::sale);
    create_message(l, buyer, MessageEvent::bought);

    db.archive_listing(l->get_listing_id());
    return true;
}

bool LogicHandler::place_bid(std::shared_ptr<Auction> l, double amount) {
    auto& u = get_current_user();
    std::vector<std::shared_ptr<Listing>> v;
    v.push_back(l);
    if(amount <= l->get_price() || u.get_balance() < amount || check_expiry(v)) return false;
    u.move_reserved(true, amount);
    auto a = std::dynamic_pointer_cast<Listing>(l);

    //Unreserve previous bidder's money
    bool exits = false;
    if(!(l->get_last_bidder() == "")) {
    auto last_u = *db.load_user(l->get_last_bidder());
        last_u.move_reserved(false, l->get_price());
        db.update_usr(last_u);
        exits = true;
    }

    if(exits) auto last_u = *db.load_user(l->get_last_bidder());
    l->set_price(amount);
    l->set_last_bidder(u.get_id());
    auto temp_u = *db.load_user(l->get_owner_id());
    create_message(a, temp_u, MessageEvent::bid);
    db.update_usr(u);
    db.update_listings_file();
    return true;
}

bool LogicHandler::negotiate(std::shared_ptr<Negotiation> l, double amount) {
    std::vector<std::shared_ptr<Listing>> v;
    v.push_back(l);

    auto& u = get_current_user();
    if(amount < 0 || u.get_balance() < amount || check_expiry(v)) return false;
    for(auto& o : l->get_offers()) {
        if(o.sender_id == u.get_id()) return false;
    }
    Offer o;
    o.sender_id = u.get_id();
    o.neg_amount = amount;
    u.move_reserved(true, amount);
    db.update_usr(u);
    l->add_offer(o);
    auto a = std::dynamic_pointer_cast<Listing>(l);
    auto temp_u = *db.load_user(l->get_owner_id());
    create_message(a, temp_u, MessageEvent::new_offer, &o);
    db.update_listings_file();
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
        db.update_usr(u);
        return true;
    }else if(method == "BankAccount") {
        u.update_bank_account(true, amount);
        u.update_balance(false, to_gorilla_coin(u.get_bank_account().curr, amount));
        db.update_usr(u);
        return true;
    }else return false;
}
bool LogicHandler::respond_offer(bool accept, std::shared_ptr<Negotiation> l, Offer& o) {
    try{
        auto sender = *db.load_user(o.sender_id);
        sender.move_reserved(false, o.neg_amount);
        auto a = std::dynamic_pointer_cast<Listing>(l);
        if(accept) {
            sender.update_balance(true, o.neg_amount);
            db.get_curr().update_balance(false, o.neg_amount);
            db.update_usr(sender);
            db.update_usr(db.get_curr());
            l->set_price(o.neg_amount);
            create_message(a, sender, MessageEvent::offer_response, &o);
            db.archive_listing(l->get_listing_id());
            return true;
        }else {
            db.update_usr(sender);
            l->delete_offer(o.sender_id);
            create_message(a, sender, MessageEvent::offer_response);
            db.update_listings_file();
            return true;
        }
    }catch(...) {return false;}
}

bool LogicHandler::check_expiry(std::vector<std::shared_ptr<Listing>>& listings) {
    std::vector<std::shared_ptr<Listing>> expired;
    for(auto l : listings) {
        if(l->get_expiry() < std::time(nullptr)) {
            //LISTING HAS EXPIRED
            expired.push_back(l);
        }
    }
    for(auto e: expired) {
        expire_listing(e);
    }
    if(expired.empty()) return false;
    return true;
}

void LogicHandler::expire_listing(std::shared_ptr<Listing>& l) {
    bool sold = false;
    l->expired = true;
    if(l->type() == "Auction") {
        auto a = std::dynamic_pointer_cast<Auction>(l);
        if(!(a->get_last_bidder() == "")) {
            auto bidder = *db.load_user(a->get_last_bidder());
            auto seller = *db.load_user(a->get_owner_id());

            double price = a->get_price();
            bidder.move_reserved(false, price);
            bidder.update_balance(true, price);
            bidder.update_balance(true, price);
            seller.update_balance(false, price);
            db.update_usr(bidder);
            db.update_usr(seller);
            create_message(l, seller, MessageEvent::sale);
            create_message(l, bidder, MessageEvent::bought);
            sold = true;
        }
    }else if(l->type() == "Negotiation") {
        auto n = std::dynamic_pointer_cast<Negotiation>(l);
        for(auto& o : n->get_offers()) {
            auto u = *db.load_user(o.sender_id);
            u.move_reserved(false, o.neg_amount);
            db.update_usr(u);
        }
    }
    auto temp_u = *db.load_user(l->get_owner_id());
    if(!sold) create_message(l, temp_u, MessageEvent::expire);
    db.archive_listing(l->get_listing_id());
    db.update_listings_file();
}

void LogicHandler::create_message(std::shared_ptr<Listing>& l, User& receiver, MessageEvent e, Offer* o) {
    std::ostringstream oss;
    Message m;
    switch(e) {
        case MessageEvent::expire:
            oss <<  "Your offer " << l->get_product().name << " has expired!";
            break;
        case MessageEvent::sale:
            oss << "Your listing " << l->get_product().name << " has been sold for " << std::fixed << std::setprecision(2) << l->get_price() << " GC!";
            break;
        case MessageEvent::new_offer:
            oss << "You have a new offer for listing " << l->get_product().name;
            if(o) {
                oss << " for " << std::fixed << std::setprecision(2) << o->neg_amount << " GC!";
            } else oss << "!";
            break;
        case MessageEvent::offer_response:
            oss << "Your offer for listing " << l->get_product().name << " has been ";
            if(o) {
                oss << "accepted! Listing has been bought for " << std::fixed << std::setprecision(2) << o->neg_amount << "!";
            }else oss << "declined!";
            break;
        case MessageEvent::bid:
            oss << "A new bid has been made for listing " << l->get_product().name << "! New price " << std::fixed << std::setprecision(2) << l->get_price() << " GC!";
            break;
        case MessageEvent::bought:
            oss << "You have bought the item from listing " << l->get_product().name << " for " << std::fixed << std::setprecision(2) << l->get_price() << " GC!";
    }
    m.body = oss.str();
    m.listing_id = l->get_listing_id();
    receiver.append_message(m);
    db.update_usr(receiver);
}