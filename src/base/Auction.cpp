#include "Auction.h"


Auction::Auction(double price, std::string owner_id, Product p) : Listing(price, owner_id, p)  {}

void to_json(nlohmann::json& j, const Auction& obj) {
    ::to_json(j, static_cast<const Listing&>(obj));
    j["last_bidder_id"] = obj.get_last_bidder();
}

void from_json(const nlohmann::json& j, Auction& obj) {
    ::from_json(j, static_cast<Listing&>(obj));
    std::string id = j.at("last_bidder_id").get<std::string>();
    obj.set_last_bidder(id);
}