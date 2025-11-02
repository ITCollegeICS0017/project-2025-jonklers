#pragma once
#include "../data/nlohmann/json.hpp"
#include <string>
#include "Listing.h"
#include <vector>
#include <memory>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

class Listing;

enum class Currency { USD, EUR, CNY, JPY, BTC, ETH, DOGE, XMR, GC };
inline std::string CurrencyToString(Currency c) {
    switch(c){
        case Currency::USD: return "USD";
        case Currency::EUR: return "EUR";
        case Currency::CNY: return "CNY";
        case Currency::JPY: return "JPY";
        case Currency::BTC: return "BTC";
        case Currency::ETH: return "ETH";
        case Currency::DOGE: return "DOGE";
        case Currency::XMR: return "XMR";
        case Currency::GC: return "GC";
        default: return "";
    } 
}

//input currency, output conversion rate to GORILLA COIN
inline const std::unordered_map<std::string, double> CONVERSION_RATE = { 
    {"USD", 0.133},
    {"EUR", 0.143},
    {"CNY", 0.019},
    {"JPY", 0.00089},
    {"BTC", 8666.67},
    {"ETH", 426.67},
    {"DOGE", 0.017},
    {"XMR", 20.0}
};

struct Wallet{
    std::string provider = "GORILLA_MONKEY_INC";
    double balance = 1000.0;
    Currency curr = Currency::BTC;
};

struct BankAccount{
    std::string provider = "GORILLA_MONKEY_INC";
    double balance = 1000.0;
    Currency curr = Currency::EUR;
};

struct Message{
    std::string body = "";
    std::string listing_id = ""; 
};

class User {
public:
    User() = default; 
    User(std::string id, std::string password);

    void update_wallet(bool subract, double amount);
    void update_bank_account(bool subract, double amount);
    void update_balance(bool subtract, double amount);
    void move_reserved(bool to_reserve, double amount);

    //Getters
    std::string get_id() const { return this->id; }
    std::string get_password() const { return this->password_hash; }
    double get_balance() const { return this->balance; }
    double get_reserved() const { return this->reserved; }
    Wallet get_wallet() const { return this->wallet; }
    BankAccount get_bank_account() const { return this->bank_account; }
    std::vector<Message> get_messages() const { return this->messages; }

    //Setters
    void set_id(std::string id) { this->id = id; }
    void set_balance(double ammount) { (ammount > 0) ? this->balance = ammount : this->balance = 0; }
    void set_reserved(double amount) { this->reserved = amount; }
    void set_password(std::string password) { this->password_hash = password; }
    void set_wallet(Wallet wallet) { this->wallet = wallet; }
    void set_bank_account(BankAccount ba) { this->bank_account = ba; }
    void set_messages(std::vector<Message> msg) { this->messages = msg; }

private:
    std::string id; 
    std::string password_hash;
    double balance = 0;
    double reserved = 0;
    Wallet wallet;
    BankAccount bank_account;
    std::vector<Message> messages;
};

std::string hash_password(const std::string &plaintext);


void to_json(nlohmann::json& j, const User& obj);
void from_json(const nlohmann::json& j, User& obj);

void to_json(nlohmann::json& j, const Wallet&obj);
void from_json(const nlohmann::json& j, Wallet& obj);

void to_json(nlohmann::json& j, const BankAccount&obj);
void from_json(const nlohmann::json& j, BankAccount& obj);

void to_json(nlohmann::json& j, const Message&obj);
void from_json(const nlohmann::json& j, Message& obj);