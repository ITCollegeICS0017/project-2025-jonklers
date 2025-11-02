#pragma once
#include "../data/nlohmann/json.hpp"
#include <string>
#include <ctime>


enum class Category { ELECTRONICS, FASHION, BOOKS, HOME, GARDEN };

struct Product {
    std::string name;
    std::string description;
    Category category;
};


class Listing {
public:
    Listing() = default;
    virtual ~Listing() = default;
    Listing(double price, std::string owner_id, Product p);

    virtual std::string type() const { return "Listing";}

    //Getters
    double get_price() const { return this->price;}
    std::string get_listing_id() const { return this->listing_id;}
    std::string get_owner_id() const { return this->owner_id;}
    std::time_t get_expiry() const { return this->expiry;}
    Product get_product() const { return this->product;}
    
    //Setters
    void set_listing_id(std::string id) {this->listing_id = id;}
    void set_price(double price) {this->price = price;}
    void set_owner_id(std::string id) {this->owner_id = id;}
    void set_expiery(std::time_t time) {this->expiry = time;}
    void set_product(Product p) {this->product = p;}

protected:
    std::string listing_id;
    double price;
    std::string owner_id;
    std::time_t expiry;
    Product product;
};

std::string gen_uuid();
inline std::time_t assign_expiry() {return std::time(nullptr) + 10 * 60;}

void to_json(nlohmann::json& j, const Listing& obj);
void from_json(const nlohmann::json& j, Listing& obj);

void to_json(nlohmann::json& j, const Product& obj);
void from_json(const nlohmann::json& j, Product& obj);