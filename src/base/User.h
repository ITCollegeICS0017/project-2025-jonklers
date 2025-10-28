#pragma once
#include "../data/nlohmann/json.hpp"
#include <string>
#include "Listing.h"
#include <vector>
#include <memory>
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>
#include <stdexcept>

class Listing;

enum class FiatCurrency { USD, EUR, CNY, JPY };
enum class CryptoCurrency { BITCOIN, ETHERIUM, DODGECOIN, MONERO };

struct Wallet{
    std::string provider;
    float balance;
    CryptoCurrency curr;
};

struct BankAccount{
    std::string provider;
    float balance;
    FiatCurrency curr;
};

struct Message{
    std::string body;
    std::string listing_id; 
};

class User {
public:
    User() = default;
    User(std::string id, Wallet wallet, BankAccount bank_account, std::vector<Message> messages);

    std::shared_ptr<Listing> create_listing();
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

std::string hash_password(const std::string &plaintext) {
    // Create a new digest context
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx)
        throw std::runtime_error("Failed to create EVP_MD_CTX");

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;

    // Initialize for SHA-256
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1)
        throw std::runtime_error("EVP_DigestInit_ex failed");

    // Feed data
    if (EVP_DigestUpdate(ctx, plaintext.data(), plaintext.size()) != 1)
        throw std::runtime_error("EVP_DigestUpdate failed");

    // Finalize digest
    if (EVP_DigestFinal_ex(ctx, hash, &length) != 1)
        throw std::runtime_error("EVP_DigestFinal_ex failed");

    // Clean up
    EVP_MD_CTX_free(ctx);

    // Convert binary hash to hex string
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < length; ++i)
        oss << std::setw(2) << static_cast<int>(hash[i]);

    return oss.str();
}


void to_json(nlohmann::json& j, const User& obj);
void from_json(const nlohmann::json& j, User& obj);

void to_json(nlohmann::json& j, const Wallet&obj);
void from_json(const nlohmann::json& j, Wallet& obj);

void to_json(nlohmann::json& j, const BankAccount&obj);
void from_json(const nlohmann::json& j, BankAccount& obj);

void to_json(nlohmann::json& j, const Message&obj);
void from_json(const nlohmann::json& j, Message& obj);