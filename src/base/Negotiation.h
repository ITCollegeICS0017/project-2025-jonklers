#pragma once
#include "Listing.h"
#include <memory>
#include <vector>

struct Offer {
    std::string sender_id;
    std::string receiver_id;
    std::string neg_amount;
};

class Negotiation: public Listing {
public:
    Negotiation() = default;
    Negotiation(std::string listing_id, double price, std::string owner_id, std::time_t expiry, std::vector<Offer> offers);

    std::vector<Offer> get_offers() const { return this->offers;}
    void set_offers(std::vector<Offer> o) { this->offers = o;}

    std::shared_ptr<Offer> make_offer();
private:
    std::vector<Offer> offers;
};

void to_json(nlohmann::json& j, const Negotiation& obj);
void from_json(const nlohmann::json& j, Negotiation& obj);

void to_json(nlohmann::json& j, const Offer& obj);
void from_json(const nlohmann::json& j, Offer& obj);