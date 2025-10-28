#pragma once
#include "Listing.h"

class Auction: public Listing {
public:
    Auction() = default;
    Auction(std::string listing_id, double price, std::string owner_id, std::time_t expiry, std::string last_bidder_id);

    std::string get_last_bidder() const { return this->last_bidder_id;}

    void set_last_bidder(std::string id) { this->last_bidder_id = id;}

    void bid();
private:
    std::string last_bidder_id;
};

void to_json(nlohmann::json& j, const Auction& obj);
void from_json(const nlohmann::json& j, Auction& obj);