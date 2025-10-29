#pragma once
#include "Listing.h"

class Auction: public Listing {
public:
    Auction() = default;
    Auction(double price, std::string owner_id, Product p);

    std::string type() const override { return "Auction";}

    std::string get_last_bidder() const { return this->last_bidder_id;}

    void set_last_bidder(std::string id) { this->last_bidder_id = id;}

private:
    std::string last_bidder_id;
};

void to_json(nlohmann::json& j, const Auction& obj);
void from_json(const nlohmann::json& j, Auction& obj);