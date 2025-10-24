#pragma once
#include <string>
#include <vector>
#include <memory>

class Listing;

enum class FiatCurrency { USD, EUR, CNY, JPY};
enum class CryptoCurrency { BITCOIN, ETHERIUM, DODGECOIN, MONERO };

struct Wallet{
   std::string owner_id;
   std::string provider;
   float balance;
   CryptoCurrency curr;
};

struct BankAccount{
   std::string owner_id;
   std::string provider;
   float balance;
   FiatCurrency curr;
};

struct Message{
    std::string message_id;
    std::string title;
    std::unique_ptr<Listing> listing; 
};

class User {
public:
   User(std::string user_id, Wallet wallet, BankAccount bank_account, std::vector<Message> messages);

   std::shared_ptr<Listing> create_listing();
   Wallet update_wallet();
   BankAccount update_bank_account();
private:
   std::string user_id; 
   std::string password_hash;
   Wallet wallet;
   BankAccount bank_account;
   std::vector<Message> messages;
};