#include "Negotiation.h"
#include <vector>

Negotiation::Negotiation(double price, std::string owner_id, Product p) : Listing(price, owner_id, p) {}


void to_json(nlohmann::json& j, const Offer& obj) {
    j = {
        {"sender_id", obj.sender_id},
        {"neg_amount", obj.neg_amount}
    };
}

void from_json(const nlohmann::json& j, Offer& obj) {
    j.at("sender_id").get_to(obj.sender_id);
    j.at("neg_amount").get_to(obj.neg_amount);
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

void Negotiation::delete_offer(std::string sender_id) {
    offers.erase(
        std::remove_if(
            offers.begin(),
            offers.end(),
            [&sender_id](const Offer& o) { return o.sender_id == sender_id; }
        ),
        offers.end()
    );
}