#include "Negotiation.h"
#include <memory>

Negotiation::Negotiation(std::string listing_id, double price, std::string owner_id, std::time_t expiry, std::vector<Offer> offers) : Listing(listing_id, price, owner_id, expiry), offers(std::move(offers)) {}


std::shared_ptr<Offer> make_offer() {
    //TODO:
    return std::make_shared<Offer>();
}