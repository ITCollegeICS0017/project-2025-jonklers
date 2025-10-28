#pragma once
#include "../data/nlohmann/json.hpp"
#include <string>
#include "Listing.h"
#include <vector>
#include <memory>
#include <iomanip>
#include <sstream>
#include <stdexcept>

class Listing;

enum class FiatCurrency { USD, EUR, CNY, JPY };
enum class CryptoCurrency { BITCOIN, ETHERIUM, DODGECOIN, MONERO };

struct Wallet{
    std::string provider = "";
    float balance = 0.0;
    CryptoCurrency curr = CryptoCurrency::BITCOIN;
};

struct BankAccount{
    std::string provider = "";
    float balance = 0.0;
    FiatCurrency curr = FiatCurrency::EUR;
};

struct Message{
    std::string body = "";
    std::string listing_id = ""; 
};

class User {
public:
    User() = default;
    User(std::string id, std::string password);

    Wallet update_wallet();
    BankAccount update_bank_account();

    //Getters
    std::string get_id() const { return this->id; }
    std::string get_password() const { return this->password_hash; }
    Wallet get_wallet() const { return this->wallet; }
    BankAccount get_bank_account() const { return this->bank_account; }
    std::vector<Message> get_messages() const { return this->messages; }

    //Setters
    void set_id(std::string id) { this->id = id; }
    void set_password(std::string password) { this->password_hash = password; }
    void set_wallet(Wallet wallet) { this->wallet = wallet; }
    void set_bank_account(BankAccount ba) { this->bank_account = ba; }
    void set_messages(std::vector<Message> msg) { this->messages = msg; }

private:
    std::string id; 
    std::string password_hash;
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