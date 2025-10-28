#include "User.h"
#include <memory>
#include <vector>
#include <openssl/evp.h>


User::User(std::string id, std::string pass) {
    this->id = id;
    this->password_hash = hash_password(pass);
}
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

std::string hash_password(std::string &plaintext){
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