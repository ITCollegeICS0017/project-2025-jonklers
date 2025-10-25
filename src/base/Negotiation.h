#pragma once
#include "Listing.h"
#include <memory>
#include <vector>

struct Offer {
    std::string sender_id;
    std::string receiver_id;
    std::string neg_amount;
};

class Negotiation: Listing {
public:
    Negotiation(std::string listing_id, std::string owner_id, std::time_t expiry, std::vector<Offer> offers);

    std::shared_ptr<Offer> make_offer();
private:
    std::vector<Offer> offers;
};