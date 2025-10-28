#include "User.h"
#include <memory>
#include <vector>


User::User(std::string user_id, Wallet wallet, BankAccount bank_account, std::vector<Message> messages) 
    : id(user_id), wallet(wallet), bank_account(bank_account), messages(messages) {} 

void to_json(nlohmann::json& j, const Wallet& w) {
    j = {
           {"provider", w.provider}, 
           {"balance", w.balance}, 
           {"currency", w.curr}
    };
}

void from_json(const nlohmann::json& j, Wallet& w) {
    j.at("provider").get_to(w.provider);
    j.at("balance").get_to(w.balance);
    j.at("currency").get_to(w.curr);
}

void to_json(nlohmann::json& j, const BankAccount& b) {
    j = {
           {"provider", b.provider}, 
           {"balance", b.balance}, 
           {"currency", b.curr}
    };
}

void from_json(const nlohmann::json& j, BankAccount& b) {
    j.at("provider").get_to(b.provider);
    j.at("balance").get_to(b.balance);
    j.at("currency").get_to(b.curr);
}

void to_json(nlohmann::json& j, const Message& m) {
    j = {
        {"body", m.body},
        {"listing_id", m.listing_id}
    };
}

void from_json(const nlohmann::json& j, Message& m) {
    j.at("body").get_to(m.body);
    j.at("listing_id").get_to(m.listing_id);
}

void to_json(nlohmann::json& j, const User& u) {
    j = {
        {"password", u.get_password()},
        {"wallet", u.get_wallet()},
        {"bank_account", u.get_bank_account()},
        {"messages", u.get_messages()}
    };
}

void from_json(const nlohmann::json& j, User& u) {
    std::string pw = j.at("password").get<std::string>();
    Wallet w = j.at("wallet").get<Wallet>();
    BankAccount b = j.at("bank_account").get<BankAccount>();
    std::vector<Message> m = j.at("messages").get<std::vector<Message>>();

    u.set_password(pw);
    u.set_wallet(w);
    u.set_bank_account(b);
    u.set_messages(m);
}
std::shared_ptr<Listing> User::create_listing() {
    //TODO:
    return std::make_shared<Listing>();
}

std::string hash_password(std::string plaintext){
    //TODO
    return "";
}