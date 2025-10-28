#include "Listing.h"

Listing::Listing(std::string listing_id, double price, std::string owner_id, std::time_t expiry)
    : listing_id(listing_id), price(price), owner_id(owner_id), expiry(expiry) {}

void to_json(nlohmann::json& j, const Listing& obj) {
    j = {
        {"price", obj.get_price()},
        {"owner_id", obj.get_owner_id()},
        {"expiry", obj.get_expiry()},
        {"product", obj.get_product()}
    };
}
void from_json(const nlohmann::json& j, Listing& obj) {
    double p = j.at("price").get<double>();
    std::string id = j.at("owner_id").get<std::string>();
    std::time_t e = j.at("expiry").get<std::time_t>();
    Product pr = j.at("product").get<Product>();

    obj.set_price(p);
    obj.set_owner_id(id);
    obj.set_expiery(e);
    obj.set_product(pr);
}

void to_json(nlohmann::json& j, const Product& obj) {
    j = {
        {"name", obj.name},
        {"description", obj.description},
        {"category", obj.category}
    };
}

void from_json(const nlohmann::json& j, Product& obj) {
    j.at("name").get_to(obj.name);
    j.at("description").get_to(obj.description);
    j.at("category").get_to(obj.category);
}