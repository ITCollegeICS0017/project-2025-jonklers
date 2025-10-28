#include "Negotiation.h"
#include <vector>

Negotiation::Negotiation(std::string listing_id, double price, std::string owner_id, std::time_t expiry, std::vector<Offer> offers) : Listing(listing_id, price, owner_id, expiry), offers(std::move(offers)) {}


void to_json(nlohmann::json& j, const Offer& obj) {
    j = {
        {"sender_id", obj.sender_id},
        {"neg_ammount", obj.neg_amount}
    };
}

void from_json(const nlohmann::json& j, Offer& obj) {
    j.at("sender_id").get_to(obj.sender_id);
    j.at("neg_ammount").get_to(obj.neg_amount);
}

void to_json(nlohmann::json& j, const Negotiation& obj) {
    ::to_json(j, static_cast<const Listing&>(obj));
    j["offers"] = obj.get_offers();
}

void from_json(const nlohmann::json& j, Negotiation& obj) {
    ::from_json(j, static_cast<Listing&>(obj));
    std::vector<Offer> o = j.at("offers").get<std::vector<Offer>>();
    obj.set_offers(o);
}