#include "User.h"

User::User(std::string user_id, Wallet wallet, BankAccount bank_account, std::vector<Message> messages) 
    : user_id(user_id), wallet(wallet), bank_account(bank_account), messages(messages) {} 

void User::to_json(nlohmann::json& j, const User& obj) {

}
//TODO:
std::shared_ptr<Listing> User::create_listing() {

}

//TODO:
Wallet User::update_wallet() {

}

//TODO:
BankAccount User::update_bank_account() {

}