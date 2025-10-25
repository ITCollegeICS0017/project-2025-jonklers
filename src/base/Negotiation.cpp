#include "Negotiation.h"

Negotiation::Negotiation(std::string listing_id, double price, std::string owner_id, std::time_t expiry, std::vector<Offer> offers) : Listing(listing_id, price, owner_id, expiry), offers(std::move(offers)) {}


//TODO:
std::shared_ptr<Offer> make_offer() {

}